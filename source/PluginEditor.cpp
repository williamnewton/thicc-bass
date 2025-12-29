#include "PluginEditor.h"

// Helper function to set up a rotary slider
static void setupRotarySlider (juce::Slider& slider, juce::Label& label, const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 18);
    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (11.0f));
}

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Apply custom look and feel
    setLookAndFeel (&customLookAndFeel);

    // === SUB OSCILLATOR ===
    setupRotarySlider (subMixSlider, subMixLabel, "SUB MIX");
    addAndMakeVisible (subMixSlider);
    addAndMakeVisible (subMixLabel);
    subMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::SUB_MIX_ID, subMixSlider);

    // === FILTER ===
    setupRotarySlider (filterCutoffSlider, filterCutoffLabel, "CUTOFF");
    addAndMakeVisible (filterCutoffSlider);
    addAndMakeVisible (filterCutoffLabel);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_CUTOFF_ID, filterCutoffSlider);

    setupRotarySlider (filterResonanceSlider, filterResonanceLabel, "RESONANCE");
    addAndMakeVisible (filterResonanceSlider);
    addAndMakeVisible (filterResonanceLabel);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_RESONANCE_ID, filterResonanceSlider);

    // === FILTER ENVELOPE ===
    setupRotarySlider (filterEnvAttackSlider, filterEnvAttackLabel, "ATTACK");
    addAndMakeVisible (filterEnvAttackSlider);
    addAndMakeVisible (filterEnvAttackLabel);
    filterEnvAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_ATTACK_ID, filterEnvAttackSlider);

    setupRotarySlider (filterEnvDecaySlider, filterEnvDecayLabel, "DECAY");
    addAndMakeVisible (filterEnvDecaySlider);
    addAndMakeVisible (filterEnvDecayLabel);
    filterEnvDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_DECAY_ID, filterEnvDecaySlider);

    setupRotarySlider (filterEnvSustainSlider, filterEnvSustainLabel, "SUSTAIN");
    addAndMakeVisible (filterEnvSustainSlider);
    addAndMakeVisible (filterEnvSustainLabel);
    filterEnvSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_SUSTAIN_ID, filterEnvSustainSlider);

    setupRotarySlider (filterEnvReleaseSlider, filterEnvReleaseLabel, "RELEASE");
    addAndMakeVisible (filterEnvReleaseSlider);
    addAndMakeVisible (filterEnvReleaseLabel);
    filterEnvReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_RELEASE_ID, filterEnvReleaseSlider);

    setupRotarySlider (filterEnvAmountSlider, filterEnvAmountLabel, "AMOUNT");
    addAndMakeVisible (filterEnvAmountSlider);
    addAndMakeVisible (filterEnvAmountLabel);
    filterEnvAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_AMOUNT_ID, filterEnvAmountSlider);

    // === AMP ENVELOPE ===
    setupRotarySlider (ampAttackSlider, ampAttackLabel, "ATTACK");
    addAndMakeVisible (ampAttackSlider);
    addAndMakeVisible (ampAttackLabel);
    ampAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_ATTACK_ID, ampAttackSlider);

    setupRotarySlider (ampDecaySlider, ampDecayLabel, "DECAY");
    addAndMakeVisible (ampDecaySlider);
    addAndMakeVisible (ampDecayLabel);
    ampDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_DECAY_ID, ampDecaySlider);

    setupRotarySlider (ampSustainSlider, ampSustainLabel, "SUSTAIN");
    addAndMakeVisible (ampSustainSlider);
    addAndMakeVisible (ampSustainLabel);
    ampSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_SUSTAIN_ID, ampSustainSlider);

    setupRotarySlider (ampReleaseSlider, ampReleaseLabel, "RELEASE");
    addAndMakeVisible (ampReleaseSlider);
    addAndMakeVisible (ampReleaseLabel);
    ampReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_RELEASE_ID, ampReleaseSlider);

    // === LFO ===
    setupRotarySlider (lfoRateSlider, lfoRateLabel, "RATE");
    addAndMakeVisible (lfoRateSlider);
    addAndMakeVisible (lfoRateLabel);
    lfoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::LFO_RATE_ID, lfoRateSlider);

    setupRotarySlider (lfoAmountSlider, lfoAmountLabel, "AMOUNT");
    addAndMakeVisible (lfoAmountSlider);
    addAndMakeVisible (lfoAmountLabel);
    lfoAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::LFO_AMOUNT_ID, lfoAmountSlider);

    // === DRIVE ===
    setupRotarySlider (driveSlider, driveLabel, "DRIVE");
    addAndMakeVisible (driveSlider);
    addAndMakeVisible (driveLabel);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::DRIVE_AMOUNT_ID, driveSlider);

    // Inspector button
    addAndMakeVisible (inspectButton);
    inspectButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a2a2a));
    inspectButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff3a3a3a));
    inspectButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xff888888));
    inspectButton.setColour (juce::TextButton::textColourOnId, juce::Colour (0xffffcc66));
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible (true);
    };

    setSize (1000, 500);
}

PluginEditor::~PluginEditor()
{
    setLookAndFeel (nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // Dark background with subtle gradient
    g.fillAll (juce::Colour (0xff0a0a0a));

    auto bounds = getLocalBounds();

    // Title bar with gradient
    auto titleArea = bounds.removeFromTop (50);
    juce::ColourGradient titleGradient (
        juce::Colour (0xff1f1f1f), titleArea.getX(), titleArea.getY(),
        juce::Colour (0xff141414), titleArea.getX(), titleArea.getBottom(),
        false);
    g.setGradientFill (titleGradient);
    g.fillRect (titleArea);

    // Title bar bottom edge highlight
    g.setColour (juce::Colour (0xffffcc66).withAlpha (0.3f));
    g.fillRect (titleArea.removeFromBottom (2));

    // Title text with shadow
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.setFont (juce::Font (28.0f, juce::Font::bold));
    g.drawText ("THICC BASS", titleArea.translated (0, 1), juce::Justification::centred);
    g.setColour (juce::Colour (0xffffcc66));
    g.drawText ("THICC BASS", titleArea, juce::Justification::centred);

    // Section background panels
    auto row1Y = 50;
    auto row2Y = 280;

    // Oscillator panel
    juce::Rectangle<int> oscPanel (5, row1Y + 25, 180, 215);
    drawSectionPanel (g, oscPanel);

    // Filter panel
    juce::Rectangle<int> filterPanel (197, row1Y + 25, 180, 215);
    drawSectionPanel (g, filterPanel);

    // Filter Envelope panel
    juce::Rectangle<int> filterEnvPanel (397, row1Y + 25, 390, 215);
    drawSectionPanel (g, filterEnvPanel);

    // Amp Envelope panel
    juce::Rectangle<int> ampPanel (807, row1Y + 25, 180, 215);
    drawSectionPanel (g, ampPanel);

    // LFO panel
    juce::Rectangle<int> lfoPanel (5, row2Y + 25, 180, 215);
    drawSectionPanel (g, lfoPanel);

    // Drive panel
    juce::Rectangle<int> drivePanel (197, row2Y + 25, 180, 215);
    drawSectionPanel (g, drivePanel);

    // Vertical dividers with gradient
    g.setColour (juce::Colour (0xff444444).withAlpha (0.5f));
    g.drawLine (192.0f, 50.0f, 192.0f, 500.0f, 1.0f);
    g.drawLine (392.0f, 50.0f, 392.0f, 500.0f, 1.0f);
    g.drawLine (802.0f, 50.0f, 802.0f, 500.0f, 1.0f);

    // Horizontal divider between rows
    g.drawLine (0.0f, 275.0f, 1000.0f, 275.0f, 1.0f);

    // Section labels with better styling
    g.setColour (juce::Colour (0xffffcc66).withAlpha (0.9f));
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText ("OSCILLATOR", 10, 60, 180, 20, juce::Justification::centred);
    g.drawText ("FILTER", 200, 60, 180, 20, juce::Justification::centred);
    g.drawText ("FILTER ENVELOPE", 400, 60, 390, 20, juce::Justification::centred);
    g.drawText ("AMP ENVELOPE", 810, 60, 180, 20, juce::Justification::centred);
    g.drawText ("LFO", 10, 290, 180, 20, juce::Justification::centred);
    g.drawText ("DRIVE", 200, 290, 180, 20, juce::Justification::centred);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (50);  // Title area

    const int knobSize = 100;
    const int labelHeight = 20;
    const int spacing = 10;

    // Row 1
    auto row1 = bounds.removeFromTop (200);

    // Oscillator Section (left)
    auto oscArea = row1.removeFromLeft (190);
    oscArea.removeFromTop (30);  // Section label space
    auto subArea = oscArea.withSizeKeepingCentre (knobSize, knobSize + labelHeight);
    subMixLabel.setBounds (subArea.removeFromBottom (labelHeight));
    subMixSlider.setBounds (subArea);

    row1.removeFromLeft (12);  // Divider space

    // Filter Section
    auto filterArea = row1.removeFromLeft (190);
    filterArea.removeFromTop (30);
    auto filterRow = filterArea.removeFromTop (knobSize + labelHeight);

    auto cutoffArea = filterRow.removeFromLeft (90);
    filterCutoffLabel.setBounds (cutoffArea.removeFromBottom (labelHeight));
    filterCutoffSlider.setBounds (cutoffArea);

    filterRow.removeFromLeft (spacing);
    filterResonanceLabel.setBounds (filterRow.removeFromBottom (labelHeight));
    filterResonanceSlider.setBounds (filterRow);

    row1.removeFromLeft (12);  // Divider space

    // Filter Envelope Section
    auto filterEnvArea = row1.removeFromLeft (390);
    filterEnvArea.removeFromTop (30);
    auto filterEnvRow = filterEnvArea.removeFromTop (knobSize + labelHeight);

    auto fAttackArea = filterEnvRow.removeFromLeft (70);
    filterEnvAttackLabel.setBounds (fAttackArea.removeFromBottom (labelHeight));
    filterEnvAttackSlider.setBounds (fAttackArea);

    filterEnvRow.removeFromLeft (spacing);
    auto fDecayArea = filterEnvRow.removeFromLeft (70);
    filterEnvDecayLabel.setBounds (fDecayArea.removeFromBottom (labelHeight));
    filterEnvDecaySlider.setBounds (fDecayArea);

    filterEnvRow.removeFromLeft (spacing);
    auto fSustainArea = filterEnvRow.removeFromLeft (70);
    filterEnvSustainLabel.setBounds (fSustainArea.removeFromBottom (labelHeight));
    filterEnvSustainSlider.setBounds (fSustainArea);

    filterEnvRow.removeFromLeft (spacing);
    auto fReleaseArea = filterEnvRow.removeFromLeft (70);
    filterEnvReleaseLabel.setBounds (fReleaseArea.removeFromBottom (labelHeight));
    filterEnvReleaseSlider.setBounds (fReleaseArea);

    filterEnvRow.removeFromLeft (spacing);
    filterEnvAmountLabel.setBounds (filterEnvRow.removeFromBottom (labelHeight));
    filterEnvAmountSlider.setBounds (filterEnvRow);

    row1.removeFromLeft (12);  // Divider space

    // Amp Envelope Section (right)
    auto ampEnvArea = row1;
    ampEnvArea.removeFromTop (30);
    auto ampEnvRow = ampEnvArea.removeFromTop (knobSize + labelHeight);

    auto aAttackArea = ampEnvRow.removeFromLeft (40);
    ampAttackLabel.setBounds (aAttackArea.removeFromBottom (labelHeight));
    ampAttackSlider.setBounds (aAttackArea);

    ampEnvRow.removeFromLeft (spacing);
    auto aDecayArea = ampEnvRow.removeFromLeft (40);
    ampDecayLabel.setBounds (aDecayArea.removeFromBottom (labelHeight));
    ampDecaySlider.setBounds (aDecayArea);

    ampEnvRow.removeFromLeft (spacing);
    auto aSustainArea = ampEnvRow.removeFromLeft (40);
    ampSustainLabel.setBounds (aSustainArea.removeFromBottom (labelHeight));
    ampSustainSlider.setBounds (aSustainArea);

    ampEnvRow.removeFromLeft (spacing);
    ampReleaseLabel.setBounds (ampEnvRow.removeFromBottom (labelHeight));
    ampReleaseSlider.setBounds (ampEnvRow);

    // Row 2 - LFO and Drive
    auto row2 = bounds.removeFromTop (200);

    // LFO Section
    auto lfoArea = row2.removeFromLeft (190);
    lfoArea.removeFromTop (30);
    auto lfoRow = lfoArea.removeFromTop (knobSize + labelHeight);

    auto rateArea = lfoRow.removeFromLeft (90);
    lfoRateLabel.setBounds (rateArea.removeFromBottom (labelHeight));
    lfoRateSlider.setBounds (rateArea);

    lfoRow.removeFromLeft (spacing);
    lfoAmountLabel.setBounds (lfoRow.removeFromBottom (labelHeight));
    lfoAmountSlider.setBounds (lfoRow);

    row2.removeFromLeft (12);  // Divider space

    // Drive Section
    auto driveArea = row2.removeFromLeft (190);
    driveArea.removeFromTop (30);
    auto dArea = driveArea.withSizeKeepingCentre (knobSize, knobSize + labelHeight);
    driveLabel.setBounds (dArea.removeFromBottom (labelHeight));
    driveSlider.setBounds (dArea);

    // Inspector button
    inspectButton.setBounds (bounds.removeFromBottom(40).withSizeKeepingCentre(100, 25));
}

void PluginEditor::drawSectionPanel (juce::Graphics& g, juce::Rectangle<int> area)
{
    // Background with subtle gradient
    juce::ColourGradient panelGradient (
        juce::Colour (0xff1a1a1a), area.getX(), area.getY(),
        juce::Colour (0xff121212), area.getX(), area.getBottom(),
        false);
    g.setGradientFill (panelGradient);
    g.fillRoundedRectangle (area.toFloat(), 4.0f);

    // Inner shadow effect
    g.setColour (juce::Colours::black.withAlpha (0.3f));
    g.drawRoundedRectangle (area.toFloat().reduced (1.0f), 4.0f, 1.0f);

    // Highlight edge
    g.setColour (juce::Colour (0xff333333).withAlpha (0.5f));
    g.drawRoundedRectangle (area.toFloat(), 4.0f, 1.0f);
}
