#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>
#include <string>

struct Preset
{
    juce::String name;

    // Oscillator
    float subMix;
    int subOctave;

    // Filter
    float filterCutoff;
    float filterResonance;
    float filterKeyTrack;

    // Filter Envelope
    float filterEnvAttack;
    float filterEnvDecay;
    float filterEnvSustain;
    float filterEnvRelease;
    float filterEnvAmount;

    // Amp Envelope
    float ampAttack;
    float ampDecay;
    float ampSustain;
    float ampRelease;

    // LFO
    float lfoRate;
    float lfoAmount;

    // Modulation
    float velocityToFilter;
    float velocityToAmp;

    // Unison
    int unisonVoices;
    float unisonDetune;

    // Misc
    float driveAmount;
    float glideTime;
};

class PresetManager
{
public:
    PresetManager()
    {
        createFactoryPresets();
    }

    const std::vector<Preset>& getPresets() const { return presets; }

    const Preset& getCurrentPreset() const
    {
        if (currentPresetIndex >= 0 && currentPresetIndex < (int)presets.size())
            return presets[currentPresetIndex];
        return presets[0];
    }

    juce::String getCurrentPresetName() const
    {
        return getCurrentPreset().name;
    }

    int getCurrentPresetIndex() const { return currentPresetIndex; }

    void nextPreset()
    {
        currentPresetIndex = (currentPresetIndex + 1) % (int)presets.size();
    }

    void previousPreset()
    {
        currentPresetIndex--;
        if (currentPresetIndex < 0)
            currentPresetIndex = (int)presets.size() - 1;
    }

    void setCurrentPresetIndex(int index)
    {
        if (index >= 0 && index < (int)presets.size())
            currentPresetIndex = index;
    }

private:
    void createFactoryPresets()
    {
        // Keep only the EPIC, ICONIC presets - 5 total

        // 1. DEEP SUB (808-style) - Pure, deep sub bass
        presets.push_back({
            "Deep Sub",
            1.0f,        // subMix - 100% sub
            1,           // subOctave - -2 octaves
            200.0f,      // filterCutoff - very low
            0.1f,        // filterResonance - minimal
            0.0f,        // filterKeyTrack
            0.001f,      // filterEnvAttack - instant
            0.05f,       // filterEnvDecay
            0.0f,        // filterEnvSustain
            0.1f,        // filterEnvRelease
            0.0f,        // filterEnvAmount - no modulation
            0.001f,      // ampAttack - instant
            0.5f,        // ampDecay - medium
            0.7f,        // ampSustain - high
            0.3f,        // ampRelease - quick
            0.0f,        // lfoRate
            0.0f,        // lfoAmount
            0.3f,        // velocityToFilter
            0.8f,        // velocityToAmp
            1,           // unisonVoices - mono
            0.0f,        // unisonDetune
            0.0f,        // driveAmount - clean
            0.0f         // glideTime
        });

        // 2. REESE BASS - Wide, detuned, movement
        presets.push_back({
            "Reese Bass",
            0.6f,        // subMix - good sub foundation
            0,           // subOctave - -1 octave
            800.0f,      // filterCutoff - mid-low
            0.3f,        // filterResonance - some character
            0.2f,        // filterKeyTrack - slight tracking
            0.01f,       // filterEnvAttack
            0.3f,        // filterEnvDecay
            0.4f,        // filterEnvSustain
            0.4f,        // filterEnvRelease
            0.6f,        // filterEnvAmount - moderate modulation
            0.005f,      // ampAttack - very quick
            0.15f,       // ampDecay
            0.85f,       // ampSustain - sustained
            0.2f,        // ampRelease
            0.3f,        // lfoRate - slow movement
            0.4f,        // lfoAmount - noticeable modulation
            0.5f,        // velocityToFilter
            0.7f,        // velocityToAmp
            5,           // unisonVoices - maximum
            0.8f,        // unisonDetune - wide detune
            0.15f,       // driveAmount - slight warmth
            0.0f         // glideTime
        });

        // 3. ACID BASS - Resonant, filter sweep classic
        presets.push_back({
            "Acid Bass",
            0.2f,        // subMix - minimal sub
            0,           // subOctave - -1 octave
            600.0f,      // filterCutoff - mid
            0.75f,       // filterResonance - high resonance
            0.3f,        // filterKeyTrack
            0.001f,      // filterEnvAttack - instant
            0.15f,       // filterEnvDecay
            0.3f,        // filterEnvSustain
            0.2f,        // filterEnvRelease
            0.95f,       // filterEnvAmount - extreme modulation
            0.001f,      // ampAttack
            0.1f,        // ampDecay
            0.6f,        // ampSustain
            0.15f,       // ampRelease
            0.0f,        // lfoRate
            0.0f,        // lfoAmount
            0.8f,        // velocityToFilter - very expressive
            0.6f,        // velocityToAmp
            1,           // unisonVoices - mono
            0.0f,        // unisonDetune
            0.1f,        // driveAmount
            0.05f        // glideTime - slight portamento
        });

        // 4. WOBBLE BASS - Fast LFO, dubstep style
        presets.push_back({
            "Wobble Bass",
            0.5f,        // subMix - balanced
            0,           // subOctave - -1 octave
            1500.0f,     // filterCutoff - mid-high
            0.55f,       // filterResonance - pronounced
            0.0f,        // filterKeyTrack
            0.001f,      // filterEnvAttack
            0.1f,        // filterEnvDecay
            0.7f,        // filterEnvSustain
            0.15f,       // filterEnvRelease
            0.3f,        // filterEnvAmount - some envelope
            0.001f,      // ampAttack
            0.05f,       // ampDecay
            1.0f,        // ampSustain - full sustain
            0.1f,        // ampRelease
            6.0f,        // lfoRate - fast wobble
            0.9f,        // lfoAmount - extreme modulation
            0.5f,        // velocityToFilter
            0.7f,        // velocityToAmp
            2,           // unisonVoices - slight width
            0.15f,       // unisonDetune
            0.4f,        // driveAmount - aggressive
            0.0f         // glideTime
        });

        // 5. THICC SAW - Maximum thicc, wide saw (Signature Sound!)
        presets.push_back({
            "Thicc Saw",
            0.3f,        // subMix - some sub support
            0,           // subOctave - -1 octave
            1800.0f,     // filterCutoff - bright
            0.25f,       // filterResonance - subtle
            0.4f,        // filterKeyTrack - tracks pitch
            0.01f,       // filterEnvAttack
            0.25f,       // filterEnvDecay
            0.6f,        // filterEnvSustain
            0.3f,        // filterEnvRelease
            0.5f,        // filterEnvAmount - moderate
            0.005f,      // ampAttack
            0.2f,        // ampDecay
            0.8f,        // ampSustain
            0.3f,        // ampRelease
            0.0f,        // lfoRate
            0.0f,        // lfoAmount
            0.6f,        // velocityToFilter
            0.7f,        // velocityToAmp
            5,           // unisonVoices - maximum
            1.0f,        // unisonDetune - maximum thicc!
            0.25f,       // driveAmount - warmth
            0.0f         // glideTime
        });

    }


    std::vector<Preset> presets;
    int currentPresetIndex = 0;
};
