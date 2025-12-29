#pragma once

#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "BinaryData.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void drawSectionPanel (juce::Graphics& g, juce::Rectangle<int> area);

    PluginProcessor& processorRef;

    // Custom LookAndFeel
    ThiccBassLookAndFeel customLookAndFeel;

    // Oscillator Section
    juce::Slider subMixSlider;
    juce::Label subMixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subMixAttachment;

    // Filter Section
    juce::Slider filterCutoffSlider;
    juce::Label filterCutoffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;

    juce::Slider filterResonanceSlider;
    juce::Label filterResonanceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;

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

    // Drive Section
    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    // Inspector for debugging
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
