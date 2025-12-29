#pragma once

#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "ThiccLogoComponent.h"
#include "OutputMeterComponent.h"
#include "WaveformVisualizerComponent.h"
#include "BinaryData.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void toggleAdvancedPanel();
    void timerCallback() override;
    void updatePresetDisplay();

    PluginProcessor& processorRef;
    bool showAdvancedPanel = false;

    // Custom LookAndFeel
    ThiccBassLookAndFeel customLookAndFeel;

    // === NEW UI COMPONENTS ===
    ThiccLogoComponent thiccLogo;
    OutputMeterComponent outputMeter;
    WaveformVisualizerComponent waveformVisualizer;
    juce::TextButton advancedButton { "ADVANCED" };

    // Preset browser
    juce::TextButton prevPresetButton { "<" };
    juce::TextButton nextPresetButton { ">" };
    juce::Label presetNameLabel;

    // === PRIMARY CONTROLS (Always Visible) ===
    // Filter Cutoff - Most important for bass shaping
    juce::Slider filterCutoffSlider;
    juce::Label filterCutoffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;

    // Filter Resonance - Character control
    juce::Slider filterResonanceSlider;
    juce::Label filterResonanceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;

    // Drive - Adds grit and presence
    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    // Sub Mix - Fundamental bass weight
    juce::Slider subMixSlider;
    juce::Label subMixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subMixAttachment;

    // THICC (Unison Detune) - Signature "thicc" sound
    juce::Slider thiccSlider;
    juce::Label thiccLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thiccAttachment;

    // Glide - Playability
    juce::Slider glideSlider;
    juce::Label glideLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> glideAttachment;

    // === SECONDARY CONTROLS (Advanced Panel) ===
    // Filter Envelope Section
    juce::Slider filterEnvAttackSlider;
    juce::Label filterEnvAttackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvAttackAttachment;

    juce::Slider filterEnvDecaySlider;
    juce::Label filterEnvDecayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvDecayAttachment;

    juce::Slider filterEnvSustainSlider;
    juce::Label filterEnvSustainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvSustainAttachment;

    juce::Slider filterEnvReleaseSlider;
    juce::Label filterEnvReleaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvReleaseAttachment;

    juce::Slider filterEnvAmountSlider;
    juce::Label filterEnvAmountLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvAmountAttachment;

    // Amp Envelope Section
    juce::Slider ampAttackSlider;
    juce::Label ampAttackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampAttackAttachment;

    juce::Slider ampDecaySlider;
    juce::Label ampDecayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampDecayAttachment;

    juce::Slider ampSustainSlider;
    juce::Label ampSustainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampSustainAttachment;

    juce::Slider ampReleaseSlider;
    juce::Label ampReleaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampReleaseAttachment;

    // LFO Section
    juce::Slider lfoRateSlider;
    juce::Label lfoRateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateAttachment;

    juce::Slider lfoAmountSlider;
    juce::Label lfoAmountLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoAmountAttachment;

    // Velocity Sensitivity
    juce::Slider velToFilterSlider;
    juce::Label velToFilterLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velToFilterAttachment;

    juce::Slider velToAmpSlider;
    juce::Label velToAmpLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velToAmpAttachment;

    // Filter Key Tracking
    juce::Slider filterKeyTrackSlider;
    juce::Label filterKeyTrackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterKeyTrackAttachment;

    // Unison Voices
    juce::Slider unisonVoicesSlider;
    juce::Label unisonVoicesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonVoicesAttachment;

    // Sub Octave Selector
    juce::ComboBox subOctaveCombo;
    juce::Label subOctaveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> subOctaveAttachment;

    // Inspector for debugging
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
