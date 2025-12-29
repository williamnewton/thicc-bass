#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override { juce::ignoreUnused(midiNoteNumber); return true; }
    bool appliesToChannel (int midiChannel) override { juce::ignoreUnused(midiChannel); return true; }
};

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice();

    bool canPlaySound (juce::SynthesiserSound* sound) override;

    void startNote (int midiNoteNumber, float velocity,
                   juce::SynthesiserSound* sound,
                   int currentPitchWheelPosition) override;

    void stopNote (float velocity, bool allowTailOff) override;

    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;

    void prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels);

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                         int startSample,
                         int numSamples) override;

    // Parameter update methods
    void setFilterCutoff (float cutoff);
    void setFilterResonance (float resonance);
    void setAmpEnvelope (float attack, float decay, float sustain, float release);
    void setFilterEnvelope (float attack, float decay, float sustain, float release);
    void setFilterEnvAmount (float amount);
    void setSubMix (float mix);
    void setLFORate (float rate);
    void setLFOAmount (float amount);
    void setDriveAmount (float drive);

private:
    // Oscillator state
    double phase = 0.0;
    double frequency = 440.0;
    double phaseDelta = 0.0;
    int currentMidiNote = -1;
    float currentVelocity = 0.0f;

    // Sub-oscillator state (one octave down, pure sine)
    double subPhase = 0.0;
    double subPhaseDelta = 0.0;
    float subMix = 0.0f;

    // Sample rate
    double currentSampleRate = 44100.0;

    // Filter (Moog ladder filter)
    using FilterType = juce::dsp::LadderFilter<float>;
    FilterType filter;

    // Smoothed filter parameters (prevents clicks/zippers)
    juce::SmoothedValue<float> smoothedCutoff;
    juce::SmoothedValue<float> smoothedResonance;

    // ADSR envelope for amplitude
    juce::ADSR ampEnvelope;
    juce::ADSR::Parameters ampEnvParams;

    // ADSR envelope for filter cutoff modulation
    juce::ADSR filterEnvelope;
    juce::ADSR::Parameters filterEnvParams;
    float filterEnvAmount = 0.0f;  // How much the envelope affects cutoff

    // LFO for filter modulation
    double lfoPhase = 0.0;
    float lfoRate = 1.0f;      // Hz
    float lfoAmount = 0.0f;    // 0-1

    // Drive/Saturation with oversampling (per bass guide: 2x)
    float driveAmount = 0.0f;  // 0-1
    juce::dsp::Oversampling<float> oversampling;

    // Temporary buffer for processing
    juce::AudioBuffer<float> tempBuffer;

    // Helper methods
    void updateFrequency();
    float generateOscillator();     // PolyBLEP sawtooth
    float generateSubOscillator();  // Pure sine wave, -1 octave
    float polyBlep (double t);      // PolyBLEP correction function

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoice)
};
