#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PresetManager.h"

#if (MSVC)
#include "ipps.h"
#endif

// Forward declaration
class SynthVoice;

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Public access to APVTS for GUI
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    // Output level metering
    float getCurrentOutputLevel() const { return currentOutputLevel.load(); }

    // Waveform capture for visualizer
    void getWaveformSamples (float* destBuffer, int numSamples);
    int getWaveformBufferSize() const { return waveformBufferSize; }

    // Preset management
    PresetManager& getPresetManager() { return presetManager; }
    void loadPreset(const Preset& preset);
    void nextPreset();
    void previousPreset();
    juce::String getCurrentPresetName() const { return presetManager.getCurrentPresetName(); }

    // Parameter IDs
    static constexpr const char* FILTER_CUTOFF_ID = "filterCutoff";
    static constexpr const char* FILTER_RESONANCE_ID = "filterResonance";
    static constexpr const char* AMP_ATTACK_ID = "ampAttack";
    static constexpr const char* AMP_DECAY_ID = "ampDecay";
    static constexpr const char* AMP_SUSTAIN_ID = "ampSustain";
    static constexpr const char* AMP_RELEASE_ID = "ampRelease";
    static constexpr const char* SUB_MIX_ID = "subMix";
    static constexpr const char* FILTER_ENV_ATTACK_ID = "filterEnvAttack";
    static constexpr const char* FILTER_ENV_DECAY_ID = "filterEnvDecay";
    static constexpr const char* FILTER_ENV_SUSTAIN_ID = "filterEnvSustain";
    static constexpr const char* FILTER_ENV_RELEASE_ID = "filterEnvRelease";
    static constexpr const char* FILTER_ENV_AMOUNT_ID = "filterEnvAmount";
    static constexpr const char* LFO_RATE_ID = "lfoRate";
    static constexpr const char* LFO_AMOUNT_ID = "lfoAmount";
    static constexpr const char* DRIVE_AMOUNT_ID = "driveAmount";

    // Phase 3 Parameter IDs
    static constexpr const char* GLIDE_TIME_ID = "glideTime";
    static constexpr const char* VELOCITY_TO_FILTER_ID = "velocityToFilter";
    static constexpr const char* VELOCITY_TO_AMP_ID = "velocityToAmp";
    static constexpr const char* FILTER_KEY_TRACK_ID = "filterKeyTrack";
    static constexpr const char* UNISON_VOICES_ID = "unisonVoices";
    static constexpr const char* UNISON_DETUNE_ID = "unisonDetune";
    static constexpr const char* SUB_OCTAVE_ID = "subOctave";

private:
    // Create APVTS parameter layout
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Update all voices with current parameter values
    void updateVoiceParameters();

    // AudioProcessorValueTreeState for parameter management
    juce::AudioProcessorValueTreeState apvts;

    // Synthesizer
    juce::Synthesiser synth;
    static constexpr int NUM_VOICES = 8;  // Polyphony

    // Output level metering (thread-safe)
    std::atomic<float> currentOutputLevel { 0.0f };

    // Waveform capture for visualizer
    juce::AudioBuffer<float> waveformBuffer;
    std::atomic<int> waveformBufferPos { 0 };
    static constexpr int waveformBufferSize = 2048;

    // Preset management
    PresetManager presetManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
