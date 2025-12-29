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
    subPhase = 0.0;

    // Phase 3: Reset unison oscillator phases
    for (auto& uniPhase : unisonPhases)
        uniPhase = 0.0;

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

    // Initialize glide smoother (Phase 3)
    glidedFrequency.reset (sampleRate, 0.1);  // Default 100ms glide
    glidedFrequency.setCurrentAndTargetValue (440.0);
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
        // === Phase 3: Update glided frequency ===
        updateGlidedFrequency();

        // Generate LFO (sine wave, -1 to 1)
        float lfoValue = std::sin (static_cast<float> (juce::MathConstants<double>::twoPi * lfoPhase));

        // Increment LFO phase
        double lfoPhaseDelta = lfoRate / currentSampleRate;
        lfoPhase += lfoPhaseDelta;
        if (lfoPhase >= 1.0)
            lfoPhase -= 1.0;

        // === Phase 3: Get envelope values with exponential curves ===
        float filterEnvValue = applyExponentialCurve (filterEnvelope.getNextSample());
        float ampEnvValue = applyExponentialCurve (ampEnvelope.getNextSample());

        // === Phase 3: Velocity sensitivity for filter ===
        float velocityFilterMod = (currentVelocity - 0.5f) * 2.0f * velocityToFilterAmount;  // -1 to +1 range
        velocityFilterMod *= 3000.0f;  // +/- 3kHz based on velocity

        // === Phase 3: Filter key tracking ===
        float keyTrackMod = 0.0f;
        if (filterKeyTrackAmount > 0.01f)
        {
            // C4 (MIDI 60) is the reference point
            float noteOffset = currentMidiNote - 60;
            keyTrackMod = noteOffset * 50.0f * filterKeyTrackAmount;  // 50 Hz per semitone at 100%
        }

        // Apply modulation to cutoff frequency
        float baseCutoff = smoothedCutoff.isSmoothing() ? smoothedCutoff.getNextValue() : smoothedCutoff.getCurrentValue();
        float envModulation = filterEnvValue * filterEnvAmount * 10000.0f;   // Envelope: up to +10kHz
        float lfoModulation = lfoValue * lfoAmount * 5000.0f;                // LFO: +/- 5kHz
        float modulatedCutoff = baseCutoff + envModulation + lfoModulation + velocityFilterMod + keyTrackMod;

        // Update filter parameters
        filter.setCutoffFrequencyHz (juce::jlimit (20.0f, 20000.0f, modulatedCutoff));

        if (smoothedResonance.isSmoothing())
            filter.setResonance (smoothedResonance.getNextValue());

        // === Phase 3: Unison - generate multiple detuned oscillators ===
        float unisonSample = 0.0f;

        for (int voice = 0; voice < unisonVoices; ++voice)
        {
            // Calculate detune amount for this voice
            float detuneCents = 0.0f;
            if (unisonVoices > 1 && unisonDetune > 0.01f)
            {
                // Spread voices evenly: -detune to +detune
                float spread = (voice / static_cast<float> (unisonVoices - 1)) - 0.5f;  // -0.5 to +0.5
                detuneCents = spread * unisonDetune * 100.0f;  // Up to +/- 100 cents at max detune
            }

            // Calculate detuned frequency
            float detuneRatio = std::pow (2.0f, detuneCents / 1200.0f);
            double detunedPhaseDelta = phaseDelta * detuneRatio;

            // Generate oscillator sample for this unison voice
            double voicePhase = (voice == 0) ? phase : unisonPhases[voice];
            float oscOutput = static_cast<float> (voicePhase * 2.0 - 1.0);
            oscOutput -= polyBlep (voicePhase);

            // Increment and wrap phase for this voice
            voicePhase += detunedPhaseDelta;
            if (voicePhase >= 1.0)
                voicePhase -= 1.0;

            // Store phase for non-primary voices
            if (voice == 0)
                phase = voicePhase;
            else
                unisonPhases[voice] = voicePhase;

            // Add to unison mix
            unisonSample += oscOutput;
        }

        // Average the unison voices
        if (unisonVoices > 1)
            unisonSample /= static_cast<float> (unisonVoices);

        // Generate sub-oscillator sample
        float subSample = generateSubOscillator();

        // Mix oscillators
        float mixedSample = unisonSample + (subSample * subMix);

        // === Phase 3: Velocity sensitivity for amp ===
        float velocityGain = 1.0f - velocityToAmpAmount + (currentVelocity * velocityToAmpAmount);
        mixedSample *= velocityGain;

        // Apply amplitude envelope
        float finalSample = mixedSample * ampEnvValue;

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

// === Phase 3 Setters ===

void SynthVoice::setGlideTime (float time)
{
    glideTime = juce::jlimit (0.0f, 2.0f, time);
    // Update glide smoothing rate based on glide time
    if (glideTime > 0.001f)
        glidedFrequency.reset (currentSampleRate, glideTime);
    else
        glidedFrequency.reset (currentSampleRate, 0.0001);  // Instant
}

void SynthVoice::setVelocityToFilter (float amount)
{
    velocityToFilterAmount = juce::jlimit (0.0f, 1.0f, amount);
}

void SynthVoice::setVelocityToAmp (float amount)
{
    velocityToAmpAmount = juce::jlimit (0.0f, 1.0f, amount);
}

void SynthVoice::setFilterKeyTracking (float amount)
{
    filterKeyTrackAmount = juce::jlimit (0.0f, 1.0f, amount);
}

void SynthVoice::setUnisonVoices (int voices)
{
    unisonVoices = juce::jlimit (1, 5, voices);
}

void SynthVoice::setUnisonDetune (float detune)
{
    unisonDetune = juce::jlimit (0.0f, 1.0f, detune);
}

void SynthVoice::setSubOctave (int octave)
{
    subOctaveDown = juce::jlimit (1, 2, octave);
}

// === Helper Methods ===

float SynthVoice::applyExponentialCurve (float linearValue)
{
    // Apply exponential shaping for more natural envelope response
    // y = x^2 gives a nice exponential feel for envelopes
    return linearValue * linearValue;
}

void SynthVoice::updateFrequency()
{
    // Convert MIDI note to frequency using equal temperament
    targetFrequency = 440.0 * std::pow (2.0, (currentMidiNote - 69) / 12.0);

    // If glide is off, jump immediately
    if (glideTime < 0.001f)
    {
        frequency = targetFrequency;
        glidedFrequency.setCurrentAndTargetValue (targetFrequency);
    }
    else
    {
        // Set target for glide
        glidedFrequency.setTargetValue (targetFrequency);
    }

    phaseDelta = frequency / currentSampleRate;

    // Sub-oscillator: 1 or 2 octaves down
    double subDivisor = (subOctaveDown == 1) ? 2.0 : 4.0;
    double subFrequency = frequency / subDivisor;
    subPhaseDelta = subFrequency / currentSampleRate;
}

void SynthVoice::updateGlidedFrequency()
{
    // Update glided frequency if gliding
    if (glidedFrequency.isSmoothing())
    {
        frequency = glidedFrequency.getNextValue();
        phaseDelta = frequency / currentSampleRate;

        // Update sub-oscillator too
        double subDivisor = (subOctaveDown == 1) ? 2.0 : 4.0;
        double subFrequency = frequency / subDivisor;
        subPhaseDelta = subFrequency / currentSampleRate;
    }
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
