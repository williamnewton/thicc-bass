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

    // === PHASE 3: GLIDE ===
    setupRotarySlider (glideSlider, glideLabel, "GLIDE");
    addAndMakeVisible (glideSlider);
    addAndMakeVisible (glideLabel);
    glideAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::GLIDE_TIME_ID, glideSlider);

    // === PHASE 3: VELOCITY SENSITIVITY ===
    setupRotarySlider (velToFilterSlider, velToFilterLabel, "VEL→FILT");
    addAndMakeVisible (velToFilterSlider);
    addAndMakeVisible (velToFilterLabel);
    velToFilterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::VELOCITY_TO_FILTER_ID, velToFilterSlider);

    setupRotarySlider (velToAmpSlider, velToAmpLabel, "VEL→AMP");
    addAndMakeVisible (velToAmpSlider);
    addAndMakeVisible (velToAmpLabel);
    velToAmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::VELOCITY_TO_AMP_ID, velToAmpSlider);

    // === PHASE 3: FILTER KEY TRACKING ===
    setupRotarySlider (filterKeyTrackSlider, filterKeyTrackLabel, "KEY TRACK");
    addAndMakeVisible (filterKeyTrackSlider);
    addAndMakeVisible (filterKeyTrackLabel);
    filterKeyTrackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_KEY_TRACK_ID, filterKeyTrackSlider);

    // === PHASE 3: UNISON ===
    setupRotarySlider (unisonVoicesSlider, unisonVoicesLabel, "VOICES");
    unisonVoicesSlider.setTextValueSuffix ("");
    addAndMakeVisible (unisonVoicesSlider);
    addAndMakeVisible (unisonVoicesLabel);
    unisonVoicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::UNISON_VOICES_ID, unisonVoicesSlider);

    setupRotarySlider (thiccSlider, thiccLabel, "THICC");
    addAndMakeVisible (thiccSlider);
    addAndMakeVisible (thiccLabel);
    thiccAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::UNISON_DETUNE_ID, thiccSlider);

    // === PHASE 3: SUB OCTAVE SELECTOR ===
    subOctaveLabel.setText ("SUB OCT", juce::dontSendNotification);
    subOctaveLabel.setJustificationType (juce::Justification::centred);
    subOctaveLabel.setFont (juce::Font (11.0f));
    addAndMakeVisible (subOctaveLabel);

    subOctaveCombo.addItem ("-1 Oct", 1);
    subOctaveCombo.addItem ("-2 Oct", 2);
    subOctaveCombo.setSelectedId (1);
    addAndMakeVisible (subOctaveCombo);
    subOctaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.getAPVTS(), PluginProcessor::SUB_OCTAVE_ID, subOctaveCombo);

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

    setSize (1200, 600);  // Phase 3: Increased for new controls
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
    auto row2Y = 240;
    auto row3Y = 430;

    // ROW 1 PANELS
    juce::Rectangle<int> oscPanel (5, row1Y + 25, 235, 155);
    drawSectionPanel (g, oscPanel);

    juce::Rectangle<int> filterPanel (250, row1Y + 25, 235, 155);
    drawSectionPanel (g, filterPanel);

    juce::Rectangle<int> filterEnvPanel (495, row1Y + 25, 390, 155);
    drawSectionPanel (g, filterEnvPanel);

    juce::Rectangle<int> ampPanel (895, row1Y + 25, 295, 155);
    drawSectionPanel (g, ampPanel);

    // ROW 2 PANELS
    juce::Rectangle<int> lfoPanel (5, row2Y + 25, 175, 155);
    drawSectionPanel (g, lfoPanel);

    juce::Rectangle<int> velPanel (190, row2Y + 25, 175, 155);
    drawSectionPanel (g, velPanel);

    juce::Rectangle<int> drivePanel (375, row2Y + 25, 100, 155);
    drawSectionPanel (g, drivePanel);

    // ROW 3 PANELS
    juce::Rectangle<int> unisonPanel (5, row3Y + 25, 175, 135);
    drawSectionPanel (g, unisonPanel);

    // Vertical dividers
    g.setColour (juce::Colour (0xff444444).withAlpha (0.5f));
    g.drawLine (245.0f, 50.0f, 245.0f, row2Y, 1.0f);
    g.drawLine (490.0f, 50.0f, 490.0f, row2Y, 1.0f);
    g.drawLine (890.0f, 50.0f, 890.0f, row2Y, 1.0f);
    g.drawLine (185.0f, row2Y, 185.0f, row3Y, 1.0f);
    g.drawLine (370.0f, row2Y, 370.0f, row3Y, 1.0f);

    // Horizontal dividers between rows
    g.drawLine (0.0f, row2Y - 5.0f, 1200.0f, row2Y - 5.0f, 1.0f);
    g.drawLine (0.0f, row3Y - 5.0f, 1200.0f, row3Y - 5.0f, 1.0f);

    // Section labels
    g.setColour (juce::Colour (0xffffcc66).withAlpha (0.9f));
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText ("OSCILLATOR", 10, 60, 230, 20, juce::Justification::centred);
    g.drawText ("FILTER", 255, 60, 230, 20, juce::Justification::centred);
    g.drawText ("FILTER ENVELOPE", 500, 60, 380, 20, juce::Justification::centred);
    g.drawText ("AMP ENVELOPE", 900, 60, 285, 20, juce::Justification::centred);
    g.drawText ("LFO", 10, row2Y + 10, 170, 20, juce::Justification::centred);
    g.drawText ("VELOCITY", 195, row2Y + 10, 170, 20, juce::Justification::centred);
    g.drawText ("DRIVE", 380, row2Y + 10, 90, 20, juce::Justification::centred);
    g.drawText ("UNISON", 10, row3Y + 10, 170, 20, juce::Justification::centred);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (50);  // Title area

    const int knobSize = 70;
    const int labelHeight = 18;
    const int spacing = 8;

    // === ROW 1 ===
    auto row1 = bounds.removeFromTop (190);

    // Oscillator Section: Sub Mix, Sub Octave, Glide
    auto oscArea = row1.removeFromLeft (240);
    oscArea.removeFromTop (30);  // Label space
    auto oscRow = oscArea.removeFromTop (knobSize + labelHeight);

    auto subMixArea = oscRow.removeFromLeft (70);
    subMixLabel.setBounds (subMixArea.removeFromBottom (labelHeight));
    subMixSlider.setBounds (subMixArea);

    oscRow.removeFromLeft (spacing);
    auto glideArea = oscRow.removeFromLeft (70);
    glideLabel.setBounds (glideArea.removeFromBottom (labelHeight));
    glideSlider.setBounds (glideArea);

    // Sub octave selector below
    auto subOctRow = oscArea.withTrimmedTop (knobSize + labelHeight + 10);
    subOctaveLabel.setBounds (subOctRow.removeFromTop (labelHeight));
    subOctaveCombo.setBounds (subOctRow.removeFromTop (25).reduced (10, 0));

    row1.removeFromLeft (10);  // Divider space

    // Filter Section: Cutoff, Resonance, Key Track
    auto filterArea = row1.removeFromLeft (240);
    filterArea.removeFromTop (30);
    auto filterRow = filterArea.removeFromTop (knobSize + labelHeight);

    auto cutoffArea = filterRow.removeFromLeft (70);
    filterCutoffLabel.setBounds (cutoffArea.removeFromBottom (labelHeight));
    filterCutoffSlider.setBounds (cutoffArea);

    filterRow.removeFromLeft (spacing);
    auto resArea = filterRow.removeFromLeft (70);
    filterResonanceLabel.setBounds (resArea.removeFromBottom (labelHeight));
    filterResonanceSlider.setBounds (resArea);

    // Key track below
    auto keyTrackRow = filterArea.withTrimmedTop (knobSize + labelHeight + 10);
    auto keyTrackArea = keyTrackRow.removeFromLeft (70);
    filterKeyTrackLabel.setBounds (keyTrackArea.removeFromBottom (labelHeight));
    filterKeyTrackSlider.setBounds (keyTrackArea);

    row1.removeFromLeft (10);  // Divider space

    // Filter Envelope: A, D, S, R, Amount
    auto filterEnvArea = row1.removeFromLeft (395);
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

    row1.removeFromLeft (10);  // Divider space

    // Amp Envelope: A, D, S, R
    auto ampEnvArea = row1;
    ampEnvArea.removeFromTop (30);
    auto ampEnvRow = ampEnvArea.removeFromTop (knobSize + labelHeight);

    auto aAttackArea = ampEnvRow.removeFromLeft (65);
    ampAttackLabel.setBounds (aAttackArea.removeFromBottom (labelHeight));
    ampAttackSlider.setBounds (aAttackArea);

    ampEnvRow.removeFromLeft (spacing);
    auto aDecayArea = ampEnvRow.removeFromLeft (65);
    ampDecayLabel.setBounds (aDecayArea.removeFromBottom (labelHeight));
    ampDecaySlider.setBounds (aDecayArea);

    ampEnvRow.removeFromLeft (spacing);
    auto aSustainArea = ampEnvRow.removeFromLeft (65);
    ampSustainLabel.setBounds (aSustainArea.removeFromBottom (labelHeight));
    ampSustainSlider.setBounds (aSustainArea);

    ampEnvRow.removeFromLeft (spacing);
    ampReleaseLabel.setBounds (ampEnvRow.removeFromBottom (labelHeight));
    ampReleaseSlider.setBounds (ampEnvRow);

    // === ROW 2 ===
    auto row2 = bounds.removeFromTop (190);

    // LFO Section
    auto lfoArea = row2.removeFromLeft (180);
    lfoArea.removeFromTop (30);
    auto lfoRow = lfoArea.removeFromTop (knobSize + labelHeight);

    auto rateArea = lfoRow.removeFromLeft (80);
    lfoRateLabel.setBounds (rateArea.removeFromBottom (labelHeight));
    lfoRateSlider.setBounds (rateArea);

    lfoRow.removeFromLeft (spacing);
    lfoAmountLabel.setBounds (lfoRow.removeFromBottom (labelHeight));
    lfoAmountSlider.setBounds (lfoRow);

    row2.removeFromLeft (10);  // Divider space

    // Velocity Section
    auto velArea = row2.removeFromLeft (180);
    velArea.removeFromTop (30);
    auto velRow = velArea.removeFromTop (knobSize + labelHeight);

    auto velFiltArea = velRow.removeFromLeft (80);
    velToFilterLabel.setBounds (velFiltArea.removeFromBottom (labelHeight));
    velToFilterSlider.setBounds (velFiltArea);

    velRow.removeFromLeft (spacing);
    velToAmpLabel.setBounds (velRow.removeFromBottom (labelHeight));
    velToAmpSlider.setBounds (velRow);

    row2.removeFromLeft (10);  // Divider space

    // Drive Section
    auto driveArea = row2.removeFromLeft (105);
    driveArea.removeFromTop (30);
    auto dArea = driveArea.withSizeKeepingCentre (knobSize, knobSize + labelHeight);
    driveLabel.setBounds (dArea.removeFromBottom (labelHeight));
    driveSlider.setBounds (dArea);

    // === ROW 3 ===
    auto row3 = bounds.removeFromTop (140);

    // Unison Section
    auto unisonArea = row3.removeFromLeft (180);
    unisonArea.removeFromTop (30);
    auto unisonRow = unisonArea.removeFromTop (knobSize + labelHeight);

    auto voicesArea = unisonRow.removeFromLeft (80);
    unisonVoicesLabel.setBounds (voicesArea.removeFromBottom (labelHeight));
    unisonVoicesSlider.setBounds (voicesArea);

    unisonRow.removeFromLeft (spacing);
    thiccLabel.setBounds (unisonRow.removeFromBottom (labelHeight));
    thiccSlider.setBounds (unisonRow);

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
