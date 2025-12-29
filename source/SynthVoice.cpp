#include "SynthVoice.h"

SynthVoice::SynthVoice()
    : oversampling (2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)  // 2x oversampling, 1 channel
{
    // Initialize filter to lowpass mode
    filter.setMode (juce::dsp::LadderFilter<float>::Mode::LPF24);

    // Set default amp envelope parameters (these will be overridden by parameters)
    ampEnvParams.attack = 0.01f;   // 10ms attack
    ampEnvParams.decay = 0.1f;     // 100ms decay
    ampEnvParams.sustain = 0.8f;   // 80% sustain level
    ampEnvParams.release = 0.1f;   // 100ms release
    ampEnvelope.setParameters (ampEnvParams);

    // Set default filter envelope parameters
    filterEnvParams.attack = 0.01f;   // 10ms attack
    filterEnvParams.decay = 0.2f;     // 200ms decay
    filterEnvParams.sustain = 0.3f;   // 30% sustain level
    filterEnvParams.release = 0.2f;   // 200ms release
    filterEnvelope.setParameters (filterEnvParams);
}

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity,
                           juce::SynthesiserSound* sound,
                           int currentPitchWheelPosition)
{
    juce::ignoreUnused (sound, currentPitchWheelPosition);

    currentMidiNote = midiNoteNumber;
    currentVelocity = velocity;

    // Update oscillator frequency based on MIDI note
    updateFrequency();

    // Reset phase to avoid clicks
    phase = 0.0;
    subPhase = 0.0;  // Reset sub-oscillator phase too

    // Trigger envelopes
    ampEnvelope.noteOn();
    filterEnvelope.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    juce::ignoreUnused (velocity);

    if (allowTailOff)
    {
        // Let the envelopes tail off naturally
        ampEnvelope.noteOff();
        filterEnvelope.noteOff();
    }
    else
    {
        // Hard stop
        clearCurrentNote();
        ampEnvelope.reset();
        filterEnvelope.reset();
    }
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    juce::ignoreUnused (newPitchWheelValue);
    // TODO: Implement pitch bend in Phase 2
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    juce::ignoreUnused (controllerNumber, newControllerValue);
    // TODO: Implement MIDI CC in Phase 2
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels)
{
    currentSampleRate = sampleRate;

    // Prepare filter
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<uint32_t> (samplesPerBlock);
    spec.numChannels = static_cast<uint32_t> (numChannels);

    filter.prepare (spec);
    filter.reset();

    // Initialize smoothed values (10ms ramp time to prevent clicks)
    smoothedCutoff.reset (sampleRate, 0.01);   // 10ms ramp
    smoothedResonance.reset (sampleRate, 0.01);

    // Set default filter parameters
    smoothedCutoff.setCurrentAndTargetValue (1000.0f);
    smoothedResonance.setCurrentAndTargetValue (0.5f);
    filter.setCutoffFrequencyHz (1000.0f);
    filter.setResonance (0.5f);

    // Prepare envelopes
    ampEnvelope.setSampleRate (sampleRate);
    ampEnvelope.reset();

    filterEnvelope.setSampleRate (sampleRate);
    filterEnvelope.reset();

    // Prepare oversampling (2x for drive/saturation)
    juce::dsp::ProcessSpec oversamplingSpec;
    oversamplingSpec.sampleRate = sampleRate;
    oversamplingSpec.maximumBlockSize = static_cast<uint32_t> (samplesPerBlock);
    oversamplingSpec.numChannels = 1;  // Mono processing
    oversampling.initProcessing (static_cast<size_t> (samplesPerBlock));
    oversampling.reset();

    // Allocate temporary buffer for processing
    tempBuffer.setSize (numChannels, samplesPerBlock);
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                                 int startSample,
                                 int numSamples)
{
    // If envelope is not active, voice is done
    if (! ampEnvelope.isActive())
    {
        clearCurrentNote();
        return;
    }

    // Render audio
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Generate LFO (sine wave, -1 to 1)
        float lfoValue = std::sin (static_cast<float> (juce::MathConstants<double>::twoPi * lfoPhase));

        // Increment LFO phase
        double lfoPhaseDelta = lfoRate / currentSampleRate;
        lfoPhase += lfoPhaseDelta;
        if (lfoPhase >= 1.0)
            lfoPhase -= 1.0;

        // Get filter envelope value (0-1)
        float filterEnvValue = filterEnvelope.getNextSample();

        // Apply modulation to cutoff frequency
        // Base cutoff + (envelope * amount) + (LFO * amount)
        float baseCutoff = smoothedCutoff.isSmoothing() ? smoothedCutoff.getNextValue() : smoothedCutoff.getCurrentValue();
        float envModulation = filterEnvValue * filterEnvAmount * 10000.0f;  // Envelope: up to +10kHz
        float lfoModulation = lfoValue * lfoAmount * 5000.0f;               // LFO: +/- 5kHz
        float modulatedCutoff = baseCutoff + envModulation + lfoModulation;

        // Update filter parameters
        filter.setCutoffFrequencyHz (juce::jlimit (20.0f, 20000.0f, modulatedCutoff));

        if (smoothedResonance.isSmoothing())
            filter.setResonance (smoothedResonance.getNextValue());

        // Generate main oscillator sample (sawtooth)
        float oscSample = generateOscillator();

        // Generate sub-oscillator sample (sine, -1 octave)
        float subSample = generateSubOscillator();

        // Mix oscillators (as per bass design guide: keep sub separate for mono)
        // Main osc + weighted sub osc
        float mixedSample = oscSample + (subSample * subMix);

        // Apply velocity
        mixedSample *= currentVelocity;

        // Get envelope value
        float envValue = ampEnvelope.getNextSample();

        // Apply envelope to sample
        float finalSample = mixedSample * envValue;

        // Write to all output channels
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample (channel, startSample + sample, finalSample);
        }
    }

    // Apply filter using DSP module (operates on blocks)
    // Create an alias of the output buffer section we're working with
    juce::dsp::AudioBlock<float> block (outputBuffer);
    auto subBlock = block.getSubBlock (static_cast<size_t> (startSample),
                                       static_cast<size_t> (numSamples));
    juce::dsp::ProcessContextReplacing<float> context (subBlock);
    filter.process (context);

    // Apply drive/saturation with oversampling (if drive > 0)
    if (driveAmount > 0.01f)
    {
        // Upsample to 2x sample rate
        auto oversampledBlock = oversampling.processSamplesUp (subBlock);

        // Apply drive (boost signal before saturation)
        float driveGain = 1.0f + (driveAmount * 9.0f);  // 1x to 10x gain

        // Apply tanh saturation (soft clipping for even harmonics)
        for (size_t channel = 0; channel < oversampledBlock.getNumChannels(); ++channel)
        {
            auto* channelData = oversampledBlock.getChannelPointer (channel);
            for (size_t i = 0; i < oversampledBlock.getNumSamples(); ++i)
            {
                channelData[i] = std::tanh (channelData[i] * driveGain);
            }
        }

        // Downsample back to original sample rate
        oversampling.processSamplesDown (subBlock);
    }
}

void SynthVoice::setFilterCutoff (float cutoff)
{
    // Set target value for smoothed cutoff (prevents clicks)
    smoothedCutoff.setTargetValue (juce::jlimit (20.0f, 20000.0f, cutoff));
}

void SynthVoice::setFilterResonance (float resonance)
{
    // Set target value for smoothed resonance (prevents clicks)
    smoothedResonance.setTargetValue (juce::jlimit (0.0f, 1.0f, resonance));
}

void SynthVoice::setAmpEnvelope (float attack, float decay, float sustain, float release)
{
    ampEnvParams.attack = attack;
    ampEnvParams.decay = decay;
    ampEnvParams.sustain = sustain;
    ampEnvParams.release = release;
    ampEnvelope.setParameters (ampEnvParams);
}

void SynthVoice::setSubMix (float mix)
{
    subMix = juce::jlimit (0.0f, 1.0f, mix);
}

void SynthVoice::setFilterEnvelope (float attack, float decay, float sustain, float release)
{
    filterEnvParams.attack = attack;
    filterEnvParams.decay = decay;
    filterEnvParams.sustain = sustain;
    filterEnvParams.release = release;
    filterEnvelope.setParameters (filterEnvParams);
}

void SynthVoice::setFilterEnvAmount (float amount)
{
    filterEnvAmount = juce::jlimit (0.0f, 1.0f, amount);
}

void SynthVoice::setLFORate (float rate)
{
    lfoRate = juce::jlimit (0.01f, 20.0f, rate);  // 0.01Hz to 20Hz
}

void SynthVoice::setLFOAmount (float amount)
{
    lfoAmount = juce::jlimit (0.0f, 1.0f, amount);
}

void SynthVoice::setDriveAmount (float drive)
{
    driveAmount = juce::jlimit (0.0f, 1.0f, drive);
}

void SynthVoice::updateFrequency()
{
    // Convert MIDI note to frequency using equal temperament
    frequency = 440.0 * std::pow (2.0, (currentMidiNote - 69) / 12.0);
    phaseDelta = frequency / currentSampleRate;

    // Sub-oscillator is one octave down (half the frequency)
    double subFrequency = frequency * 0.5;
    subPhaseDelta = subFrequency / currentSampleRate;
}

float SynthVoice::generateOscillator()
{
    // PolyBLEP sawtooth oscillator
    // Phase goes from 0 to 1
    float output = static_cast<float> (phase * 2.0 - 1.0);  // Convert to -1 to 1

    // Apply polyBLEP to smooth discontinuities
    // Check for discontinuity at phase wrap (near 0)
    output -= polyBlep (phase);

    // Increment phase
    phase += phaseDelta;

    // Wrap phase
    if (phase >= 1.0)
        phase -= 1.0;

    return output;
}

float SynthVoice::polyBlep (double t)
{
    // PolyBLEP (Polynomial Bandlimited Step) algorithm
    // Reduces aliasing by smoothing discontinuities

    double dt = phaseDelta;

    // Check for discontinuity near 0 (phase wrap)
    if (t < dt)
    {
        // Use polynomial to smooth the step
        t /= dt;
        return static_cast<float> (t + t - t * t - 1.0);
    }
    // Check for discontinuity near 1 (before wrap)
    else if (t > 1.0 - dt)
    {
        // Use polynomial to smooth the step
        t = (t - 1.0) / dt;
        return static_cast<float> (t * t + t + t + 1.0);
    }

    // No discontinuity, return 0
    return 0.0f;
}

float SynthVoice::generateSubOscillator()
{
    // Pure sine wave for sub-bass (no polyBLEP needed for sine)
    float output = std::sin (static_cast<float> (juce::MathConstants<double>::twoPi * subPhase));

    // Increment phase
    subPhase += subPhaseDelta;

    // Wrap phase
    if (subPhase >= 1.0)
        subPhase -= 1.0;

    return output;
}
