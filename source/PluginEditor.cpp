#include "PluginEditor.h"

// Helper function to set up a large rotary slider for primary controls
static void setupPrimarySlider (juce::Slider& slider, juce::Label& label, const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 20);
    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (13.0f, juce::Font::bold));
}

// Helper function to set up a standard rotary slider for secondary controls
static void setupSecondarySlider (juce::Slider& slider, juce::Label& label, const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 65, 18);
    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (10.0f, juce::Font::bold));
}

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Apply custom look and feel
    setLookAndFeel (&customLookAndFeel);

    // === NEW UI COMPONENTS ===
    addAndMakeVisible (thiccLogo);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (waveformVisualizer);

    advancedButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffFF6600));  // Orange
    advancedButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffffdd00));  // Yellow when active
    advancedButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xffffffff));
    advancedButton.setColour (juce::TextButton::textColourOnId, juce::Colour (0xff000000));
    advancedButton.onClick = [this]() { toggleAdvancedPanel(); };
    addAndMakeVisible (advancedButton);

    // Preset browser - street art style with vibrant colors
    prevPresetButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffFF3333));  // Red
    prevPresetButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xffffffff));
    prevPresetButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffFF6600));  // Orange when pressed
    prevPresetButton.setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff000000));
    prevPresetButton.onClick = [this]() {
        processorRef.previousPreset();
        updatePresetDisplay();
    };
    addAndMakeVisible (prevPresetButton);

    nextPresetButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffFF3333));  // Red
    nextPresetButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xffffffff));
    nextPresetButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffFF6600));  // Orange when pressed
    nextPresetButton.setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff000000));
    nextPresetButton.onClick = [this]() {
        processorRef.nextPreset();
        updatePresetDisplay();
    };
    addAndMakeVisible (nextPresetButton);

    presetNameLabel.setColour (juce::Label::textColourId, juce::Colour (0xff000000));
    presetNameLabel.setColour (juce::Label::backgroundColourId, juce::Colour (0xffffffff));  // White background
    presetNameLabel.setColour (juce::Label::outlineColourId, juce::Colour (0xff000000));
    presetNameLabel.setJustificationType (juce::Justification::centred);
    presetNameLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    addAndMakeVisible (presetNameLabel);

    updatePresetDisplay();

    // === PRIMARY CONTROLS SETUP ===
    // Filter Cutoff
    setupPrimarySlider (filterCutoffSlider, filterCutoffLabel, "CUTOFF");
    filterCutoffSlider.setTooltip ("Filter cutoff frequency (20Hz - 20kHz)\nShapes the tone by removing high frequencies");
    addAndMakeVisible (filterCutoffSlider);
    addAndMakeVisible (filterCutoffLabel);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_CUTOFF_ID, filterCutoffSlider);

    // Filter Resonance
    setupPrimarySlider (filterResonanceSlider, filterResonanceLabel, "RESONANCE");
    filterResonanceSlider.setTooltip ("Filter resonance (0-100%)\nAdds emphasis at the cutoff frequency for character and edge");
    addAndMakeVisible (filterResonanceSlider);
    addAndMakeVisible (filterResonanceLabel);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_RESONANCE_ID, filterResonanceSlider);

    // Drive
    setupPrimarySlider (driveSlider, driveLabel, "DRIVE");
    driveSlider.setTooltip ("Distortion amount (0-100%)\nAdds warmth, grit, and harmonic richness");
    addAndMakeVisible (driveSlider);
    addAndMakeVisible (driveLabel);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::DRIVE_AMOUNT_ID, driveSlider);

    // Sub Mix
    setupPrimarySlider (subMixSlider, subMixLabel, "SUB MIX");
    subMixSlider.setTooltip ("Sub oscillator level (0-100%)\nBlends in low-frequency sub bass for weight and power");
    addAndMakeVisible (subMixSlider);
    addAndMakeVisible (subMixLabel);
    subMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::SUB_MIX_ID, subMixSlider);

    // THICC (Unison Detune)
    setupPrimarySlider (thiccSlider, thiccLabel, "THICC");
    thiccSlider.setTooltip ("Unison detune amount (0-100%)\nMakes the sound wider and thicker - signature THICC sound!");
    addAndMakeVisible (thiccSlider);
    addAndMakeVisible (thiccLabel);
    thiccAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::UNISON_DETUNE_ID, thiccSlider);

    // Glide
    setupPrimarySlider (glideSlider, glideLabel, "GLIDE");
    glideSlider.setTooltip ("Portamento time (0-2 seconds)\nSmooth pitch glide between notes for expressive playing");
    addAndMakeVisible (glideSlider);
    addAndMakeVisible (glideLabel);
    glideAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::GLIDE_TIME_ID, glideSlider);

    // === SECONDARY CONTROLS SETUP (Advanced Panel) ===
    // Filter Envelope
    setupSecondarySlider (filterEnvAttackSlider, filterEnvAttackLabel, "F-ATTACK");
    filterEnvAttackSlider.setTooltip ("Filter envelope attack time\nHow quickly the filter opens after note on");
    filterEnvAttackSlider.setVisible (false);
    filterEnvAttackLabel.setVisible (false);
    addAndMakeVisible (filterEnvAttackSlider);
    addAndMakeVisible (filterEnvAttackLabel);
    filterEnvAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_ATTACK_ID, filterEnvAttackSlider);

    setupSecondarySlider (filterEnvDecaySlider, filterEnvDecayLabel, "F-DECAY");
    filterEnvDecaySlider.setTooltip ("Filter envelope decay time\nHow quickly the filter moves to sustain level");
    filterEnvDecaySlider.setVisible (false);
    filterEnvDecayLabel.setVisible (false);
    addAndMakeVisible (filterEnvDecaySlider);
    addAndMakeVisible (filterEnvDecayLabel);
    filterEnvDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_DECAY_ID, filterEnvDecaySlider);

    setupSecondarySlider (filterEnvSustainSlider, filterEnvSustainLabel, "F-SUSTAIN");
    filterEnvSustainSlider.setTooltip ("Filter envelope sustain level\nFilter modulation amount while note is held");
    filterEnvSustainSlider.setVisible (false);
    filterEnvSustainLabel.setVisible (false);
    addAndMakeVisible (filterEnvSustainSlider);
    addAndMakeVisible (filterEnvSustainLabel);
    filterEnvSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_SUSTAIN_ID, filterEnvSustainSlider);

    setupSecondarySlider (filterEnvReleaseSlider, filterEnvReleaseLabel, "F-RELEASE");
    filterEnvReleaseSlider.setTooltip ("Filter envelope release time\nHow quickly the filter closes after note off");
    filterEnvReleaseSlider.setVisible (false);
    filterEnvReleaseLabel.setVisible (false);
    addAndMakeVisible (filterEnvReleaseSlider);
    addAndMakeVisible (filterEnvReleaseLabel);
    filterEnvReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_RELEASE_ID, filterEnvReleaseSlider);

    setupSecondarySlider (filterEnvAmountSlider, filterEnvAmountLabel, "F-AMOUNT");
    filterEnvAmountSlider.setTooltip ("Filter envelope amount\nHow much the envelope modulates the filter cutoff");
    filterEnvAmountSlider.setVisible (false);
    filterEnvAmountLabel.setVisible (false);
    addAndMakeVisible (filterEnvAmountSlider);
    addAndMakeVisible (filterEnvAmountLabel);
    filterEnvAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_ENV_AMOUNT_ID, filterEnvAmountSlider);

    // Amp Envelope
    setupSecondarySlider (ampAttackSlider, ampAttackLabel, "A-ATTACK");
    ampAttackSlider.setTooltip ("Amplitude envelope attack time\nHow quickly the sound fades in after note on");
    ampAttackSlider.setVisible (false);
    ampAttackLabel.setVisible (false);
    addAndMakeVisible (ampAttackSlider);
    addAndMakeVisible (ampAttackLabel);
    ampAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_ATTACK_ID, ampAttackSlider);

    setupSecondarySlider (ampDecaySlider, ampDecayLabel, "A-DECAY");
    ampDecaySlider.setTooltip ("Amplitude envelope decay time\nHow quickly the volume moves to sustain level");
    ampDecaySlider.setVisible (false);
    ampDecayLabel.setVisible (false);
    addAndMakeVisible (ampDecaySlider);
    addAndMakeVisible (ampDecayLabel);
    ampDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_DECAY_ID, ampDecaySlider);

    setupSecondarySlider (ampSustainSlider, ampSustainLabel, "A-SUSTAIN");
    ampSustainSlider.setTooltip ("Amplitude envelope sustain level\nVolume level while note is held");
    ampSustainSlider.setVisible (false);
    ampSustainLabel.setVisible (false);
    addAndMakeVisible (ampSustainSlider);
    addAndMakeVisible (ampSustainLabel);
    ampSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_SUSTAIN_ID, ampSustainSlider);

    setupSecondarySlider (ampReleaseSlider, ampReleaseLabel, "A-RELEASE");
    ampReleaseSlider.setTooltip ("Amplitude envelope release time\nHow quickly the sound fades out after note off");
    ampReleaseSlider.setVisible (false);
    ampReleaseLabel.setVisible (false);
    addAndMakeVisible (ampReleaseSlider);
    addAndMakeVisible (ampReleaseLabel);
    ampReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::AMP_RELEASE_ID, ampReleaseSlider);

    // LFO
    setupSecondarySlider (lfoRateSlider, lfoRateLabel, "LFO RATE");
    lfoRateSlider.setTooltip ("LFO speed (0.1 - 20 Hz)\nCyclic modulation rate for wobble and movement");
    lfoRateSlider.setVisible (false);
    lfoRateLabel.setVisible (false);
    addAndMakeVisible (lfoRateSlider);
    addAndMakeVisible (lfoRateLabel);
    lfoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::LFO_RATE_ID, lfoRateSlider);

    setupSecondarySlider (lfoAmountSlider, lfoAmountLabel, "LFO AMT");
    lfoAmountSlider.setTooltip ("LFO modulation depth\nHow much the LFO affects the filter cutoff");
    lfoAmountSlider.setVisible (false);
    lfoAmountLabel.setVisible (false);
    addAndMakeVisible (lfoAmountSlider);
    addAndMakeVisible (lfoAmountLabel);
    lfoAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::LFO_AMOUNT_ID, lfoAmountSlider);

    // Velocity
    setupSecondarySlider (velToFilterSlider, velToFilterLabel, "VEL→FILT");
    velToFilterSlider.setTooltip ("Velocity to filter sensitivity\nHow much key velocity affects filter brightness");
    velToFilterSlider.setVisible (false);
    velToFilterLabel.setVisible (false);
    addAndMakeVisible (velToFilterSlider);
    addAndMakeVisible (velToFilterLabel);
    velToFilterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::VELOCITY_TO_FILTER_ID, velToFilterSlider);

    setupSecondarySlider (velToAmpSlider, velToAmpLabel, "VEL→AMP");
    velToAmpSlider.setTooltip ("Velocity to amplitude sensitivity\nHow much key velocity affects volume");
    velToAmpSlider.setVisible (false);
    velToAmpLabel.setVisible (false);
    addAndMakeVisible (velToAmpSlider);
    addAndMakeVisible (velToAmpLabel);
    velToAmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::VELOCITY_TO_AMP_ID, velToAmpSlider);

    // Filter Key Tracking
    setupSecondarySlider (filterKeyTrackSlider, filterKeyTrackLabel, "KEY TRACK");
    filterKeyTrackSlider.setTooltip ("Filter keyboard tracking\nMakes filter follow played note pitch (0% = off, 100% = full tracking)");
    filterKeyTrackSlider.setVisible (false);
    filterKeyTrackLabel.setVisible (false);
    addAndMakeVisible (filterKeyTrackSlider);
    addAndMakeVisible (filterKeyTrackLabel);
    filterKeyTrackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::FILTER_KEY_TRACK_ID, filterKeyTrackSlider);

    // Unison Voices
    setupSecondarySlider (unisonVoicesSlider, unisonVoicesLabel, "VOICES");
    unisonVoicesSlider.setTooltip ("Number of unison voices (1-5)\nMore voices = thicker sound (works with THICC control)");
    unisonVoicesSlider.setVisible (false);
    unisonVoicesLabel.setVisible (false);
    addAndMakeVisible (unisonVoicesSlider);
    addAndMakeVisible (unisonVoicesLabel);
    unisonVoicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), PluginProcessor::UNISON_VOICES_ID, unisonVoicesSlider);

    // Sub Octave Selector
    subOctaveLabel.setText ("SUB OCT", juce::dontSendNotification);
    subOctaveLabel.setJustificationType (juce::Justification::centred);
    subOctaveLabel.setFont (juce::Font (10.0f, juce::Font::bold));
    subOctaveLabel.setVisible (false);
    addAndMakeVisible (subOctaveLabel);

    subOctaveCombo.addItem ("-1 Oct", 1);
    subOctaveCombo.addItem ("-2 Oct", 2);
    subOctaveCombo.setSelectedId (1);
    subOctaveCombo.setTooltip ("Sub oscillator octave\nChoose how far below the main oscillator the sub plays");
    subOctaveCombo.setVisible (false);
    addAndMakeVisible (subOctaveCombo);
    subOctaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.getAPVTS(), PluginProcessor::SUB_OCTAVE_ID, subOctaveCombo);

    // Inspector button
    addAndMakeVisible (inspectButton);
    inspectButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffdddddd));
    inspectButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffffdd00));
    inspectButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xff666666));
    inspectButton.setColour (juce::TextButton::textColourOnId, juce::Colour (0xff000000));
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible (true);
    };

    // Window size - compact single-line interface with visualizer
    setSize (1200, 220);

    // Start timer to update output meter
    startTimerHz (30);  // 30 FPS refresh rate
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

void PluginEditor::timerCallback()
{
    // Update output meter with current level from processor
    float level = processorRef.getCurrentOutputLevel();
    outputMeter.setLevel (level);

    // Update waveform visualizer with audio samples
    const int bufferSize = 512;
    float sampleBuffer[bufferSize];
    processorRef.getWaveformSamples (sampleBuffer, bufferSize);
    waveformVisualizer.pushSamples (sampleBuffer, bufferSize);
}

void PluginEditor::updatePresetDisplay()
{
    presetNameLabel.setText (processorRef.getCurrentPresetName(), juce::dontSendNotification);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // Clean white background for logo contrast (street art aesthetic)
    auto bounds = getLocalBounds();

    // Base white background
    g.fillAll (juce::Colour (0xffffffff));

    // DECORATIVE BORDER - Comic book / Street art style
    const float inset = 5.0f;
    const float borderThickness = 4.0f;
    const float dotRadius = 5.0f;  // Smaller dots

    auto frameBounds = bounds.toFloat().reduced (inset);

    // Draw thick black border frame
    g.setColour (juce::Colours::black);
    g.drawRect (frameBounds, borderThickness);

    // Yellow dots - ONLY AT 4 CORNERS
    juce::Colour dotColor (0xffffdd00);  // Bright yellow

    // Top-left
    g.setColour (juce::Colours::black);
    g.fillEllipse (frameBounds.getX() - dotRadius, frameBounds.getY() - dotRadius, dotRadius * 2, dotRadius * 2);
    g.setColour (dotColor);
    g.fillEllipse (frameBounds.getX() - dotRadius + 1.5f, frameBounds.getY() - dotRadius + 1.5f, (dotRadius - 1.5f) * 2, (dotRadius - 1.5f) * 2);

    // Top-right
    g.setColour (juce::Colours::black);
    g.fillEllipse (frameBounds.getRight() - dotRadius, frameBounds.getY() - dotRadius, dotRadius * 2, dotRadius * 2);
    g.setColour (dotColor);
    g.fillEllipse (frameBounds.getRight() - dotRadius + 1.5f, frameBounds.getY() - dotRadius + 1.5f, (dotRadius - 1.5f) * 2, (dotRadius - 1.5f) * 2);

    // Bottom-left
    g.setColour (juce::Colours::black);
    g.fillEllipse (frameBounds.getX() - dotRadius, frameBounds.getBottom() - dotRadius, dotRadius * 2, dotRadius * 2);
    g.setColour (dotColor);
    g.fillEllipse (frameBounds.getX() - dotRadius + 1.5f, frameBounds.getBottom() - dotRadius + 1.5f, (dotRadius - 1.5f) * 2, (dotRadius - 1.5f) * 2);

    // Bottom-right
    g.setColour (juce::Colours::black);
    g.fillEllipse (frameBounds.getRight() - dotRadius, frameBounds.getBottom() - dotRadius, dotRadius * 2, dotRadius * 2);
    g.setColour (dotColor);
    g.fillEllipse (frameBounds.getRight() - dotRadius + 1.5f, frameBounds.getBottom() - dotRadius + 1.5f, (dotRadius - 1.5f) * 2, (dotRadius - 1.5f) * 2);

    // If advanced panel is showing, draw divider
    if (showAdvancedPanel)
    {
        // Dividing line
        g.setColour (juce::Colour (0xff333333));
        g.drawLine (0.0f, 220.0f, static_cast<float>(getWidth()), 220.0f, 2.0f);
    }
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    const int margin = 10;
    const int primaryKnobSize = 100;
    const int primaryLabelHeight = 20;

    // === SINGLE LINE MAIN CONTROLS ===
    auto mainRow = bounds.removeFromTop (220);
    int currentX = margin;

    // THICC Logo (left) - now wider for custom logo
    thiccLogo.setBounds (currentX, margin, 200, 140);
    currentX += 200 + margin * 2;

    // 6 Primary Knobs in a row
    const int knobSpacing = 8;
    const int textBoxHeight = 20;  // Height for value display below knob

    // Filter Cutoff - label above, knob + value below
    auto cutoffArea = juce::Rectangle<int> (currentX, 20, primaryKnobSize, primaryKnobSize + textBoxHeight + primaryLabelHeight);
    filterCutoffLabel.setBounds (cutoffArea.removeFromTop (primaryLabelHeight));
    filterCutoffSlider.setBounds (cutoffArea);
    currentX += primaryKnobSize + knobSpacing;

    // Filter Resonance
    auto resArea = juce::Rectangle<int> (currentX, 20, primaryKnobSize, primaryKnobSize + textBoxHeight + primaryLabelHeight);
    filterResonanceLabel.setBounds (resArea.removeFromTop (primaryLabelHeight));
    filterResonanceSlider.setBounds (resArea);
    currentX += primaryKnobSize + knobSpacing;

    // Drive
    auto driveArea = juce::Rectangle<int> (currentX, 20, primaryKnobSize, primaryKnobSize + textBoxHeight + primaryLabelHeight);
    driveLabel.setBounds (driveArea.removeFromTop (primaryLabelHeight));
    driveSlider.setBounds (driveArea);
    currentX += primaryKnobSize + knobSpacing;

    // Sub Mix
    auto subArea = juce::Rectangle<int> (currentX, 20, primaryKnobSize, primaryKnobSize + textBoxHeight + primaryLabelHeight);
    subMixLabel.setBounds (subArea.removeFromTop (primaryLabelHeight));
    subMixSlider.setBounds (subArea);
    currentX += primaryKnobSize + knobSpacing;

    // THICC (Unison Detune)
    auto thiccArea = juce::Rectangle<int> (currentX, 20, primaryKnobSize, primaryKnobSize + textBoxHeight + primaryLabelHeight);
    thiccLabel.setBounds (thiccArea.removeFromTop (primaryLabelHeight));
    thiccSlider.setBounds (thiccArea);
    currentX += primaryKnobSize + knobSpacing;

    // Glide
    auto glideArea = juce::Rectangle<int> (currentX, 20, primaryKnobSize, primaryKnobSize + textBoxHeight + primaryLabelHeight);
    glideLabel.setBounds (glideArea.removeFromTop (primaryLabelHeight));
    glideSlider.setBounds (glideArea);
    currentX += primaryKnobSize + margin * 2;

    // Output Meter - same size as other knobs (100px)
    const int meterX = currentX;
    outputMeter.setBounds (meterX, 20, primaryKnobSize, primaryKnobSize);

    // Waveform visualizer - directly to the right of output meter
    waveformVisualizer.setBounds (meterX + primaryKnobSize + knobSpacing, 20, primaryKnobSize + 20, primaryKnobSize);

    // Advanced button (below output meter/waveform on right)
    advancedButton.setBounds (meterX, 20 + primaryKnobSize + 10, 100, 30);

    // Preset browser (centered, positioned away from bottom edge)
    const int presetBrowserWidth = 300;
    const int presetBrowserX = (getWidth() - presetBrowserWidth) / 2;
    const int presetBrowserY = 165;  // Moved up to avoid bottom edge overlap
    const int buttonHeight = 30;     // Taller buttons for better visibility

    prevPresetButton.setBounds (presetBrowserX, presetBrowserY, 50, buttonHeight);
    presetNameLabel.setBounds (presetBrowserX + 55, presetBrowserY, presetBrowserWidth - 110, buttonHeight);
    nextPresetButton.setBounds (presetBrowserX + presetBrowserWidth - 50, presetBrowserY, 50, buttonHeight);

    // === ADVANCED PANEL (if showing) ===
    if (showAdvancedPanel)
    {
        // Advanced panel starts AFTER the main row (at Y=220)
        const int secondaryKnobSize = 70;
        const int secondaryLabelHeight = 20;
        const int secondaryTextBoxHeight = 18;  // Height for value display
        const int secondarySpacing = 12;
        const int sectionSpacing = 35;

        // Start positioning from the top of the advanced panel area
        int panelY = 220 + 25;  // Main row height (220) + space for section label (25)

        // Calculate total width to center the layout
        // Row 1: Filter Env (5 knobs) + Amp Env (4 knobs) = 9 knobs total
        int row1TotalWidth = (secondaryKnobSize * 9) + (secondarySpacing * 8) + sectionSpacing;
        int row1StartX = (getWidth() - row1TotalWidth) / 2;

        // FILTER ENVELOPE Section
        int filterEnvX = row1StartX;
        int filterEnvY = panelY;

        auto fAttackArea = juce::Rectangle<int> (filterEnvX, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        filterEnvAttackLabel.setBounds (fAttackArea.removeFromTop (secondaryLabelHeight));
        filterEnvAttackSlider.setBounds (fAttackArea);

        auto fDecayArea = juce::Rectangle<int> (filterEnvX + (secondaryKnobSize + secondarySpacing), filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        filterEnvDecayLabel.setBounds (fDecayArea.removeFromTop (secondaryLabelHeight));
        filterEnvDecaySlider.setBounds (fDecayArea);

        auto fSustainArea = juce::Rectangle<int> (filterEnvX + (secondaryKnobSize + secondarySpacing) * 2, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        filterEnvSustainLabel.setBounds (fSustainArea.removeFromTop (secondaryLabelHeight));
        filterEnvSustainSlider.setBounds (fSustainArea);

        auto fReleaseArea = juce::Rectangle<int> (filterEnvX + (secondaryKnobSize + secondarySpacing) * 3, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        filterEnvReleaseLabel.setBounds (fReleaseArea.removeFromTop (secondaryLabelHeight));
        filterEnvReleaseSlider.setBounds (fReleaseArea);

        auto fAmountArea = juce::Rectangle<int> (filterEnvX + (secondaryKnobSize + secondarySpacing) * 4, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        filterEnvAmountLabel.setBounds (fAmountArea.removeFromTop (secondaryLabelHeight));
        filterEnvAmountSlider.setBounds (fAmountArea);

        // AMP ENVELOPE Section
        int ampEnvX = filterEnvX + (secondaryKnobSize + secondarySpacing) * 5 + sectionSpacing;

        auto aAttackArea = juce::Rectangle<int> (ampEnvX, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        ampAttackLabel.setBounds (aAttackArea.removeFromTop (secondaryLabelHeight));
        ampAttackSlider.setBounds (aAttackArea);

        auto aDecayArea = juce::Rectangle<int> (ampEnvX + (secondaryKnobSize + secondarySpacing), filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        ampDecayLabel.setBounds (aDecayArea.removeFromTop (secondaryLabelHeight));
        ampDecaySlider.setBounds (aDecayArea);

        auto aSustainArea = juce::Rectangle<int> (ampEnvX + (secondaryKnobSize + secondarySpacing) * 2, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        ampSustainLabel.setBounds (aSustainArea.removeFromTop (secondaryLabelHeight));
        ampSustainSlider.setBounds (aSustainArea);

        auto aReleaseArea = juce::Rectangle<int> (ampEnvX + (secondaryKnobSize + secondarySpacing) * 3, filterEnvY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        ampReleaseLabel.setBounds (aReleaseArea.removeFromTop (secondaryLabelHeight));
        ampReleaseSlider.setBounds (aReleaseArea);

        // BOTTOM ROW: LFO | VELOCITY | MISC
        panelY += secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight + 20;

        // Calculate total width for Row 2 to center it
        // LFO (2) + Velocity (2) + Misc (3) = 7 knobs total
        int row2TotalWidth = (secondaryKnobSize * 7) + (secondarySpacing * 6) + (sectionSpacing * 2);
        int row2StartX = (getWidth() - row2TotalWidth) / 2;

        // LFO Section
        int lfoX = row2StartX;
        auto lfoRateArea = juce::Rectangle<int> (lfoX, panelY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        lfoRateLabel.setBounds (lfoRateArea.removeFromTop (secondaryLabelHeight));
        lfoRateSlider.setBounds (lfoRateArea);

        auto lfoAmtArea = juce::Rectangle<int> (lfoX + (secondaryKnobSize + secondarySpacing), panelY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        lfoAmountLabel.setBounds (lfoAmtArea.removeFromTop (secondaryLabelHeight));
        lfoAmountSlider.setBounds (lfoAmtArea);

        // Velocity Section
        int velX = lfoX + (secondaryKnobSize + secondarySpacing) * 2 + sectionSpacing;

        auto velFiltArea = juce::Rectangle<int> (velX, panelY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        velToFilterLabel.setBounds (velFiltArea.removeFromTop (secondaryLabelHeight));
        velToFilterSlider.setBounds (velFiltArea);

        auto velAmpArea = juce::Rectangle<int> (velX + (secondaryKnobSize + secondarySpacing), panelY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        velToAmpLabel.setBounds (velAmpArea.removeFromTop (secondaryLabelHeight));
        velToAmpSlider.setBounds (velAmpArea);

        // Misc Section
        int miscX = velX + (secondaryKnobSize + secondarySpacing) * 2 + sectionSpacing;

        auto keyTrackArea = juce::Rectangle<int> (miscX, panelY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        filterKeyTrackLabel.setBounds (keyTrackArea.removeFromTop (secondaryLabelHeight));
        filterKeyTrackSlider.setBounds (keyTrackArea);

        auto voicesArea = juce::Rectangle<int> (miscX + (secondaryKnobSize + secondarySpacing), panelY, secondaryKnobSize, secondaryKnobSize + secondaryTextBoxHeight + secondaryLabelHeight);
        unisonVoicesLabel.setBounds (voicesArea.removeFromTop (secondaryLabelHeight));
        unisonVoicesSlider.setBounds (voicesArea);

        // Sub Octave ComboBox
        int subOctX = miscX + (secondaryKnobSize + secondarySpacing) * 2;
        subOctaveLabel.setBounds (subOctX, panelY, secondaryKnobSize, secondaryLabelHeight);
        subOctaveCombo.setBounds (subOctX, panelY + secondaryLabelHeight + 5, secondaryKnobSize, 30);

        // Inspector button - bottom-left corner of advanced panel
        inspectButton.setBounds (20, getHeight() - 35, 80, 25);
    }
    else
    {
        // Inspector button - hidden when advanced panel is closed
        inspectButton.setBounds (0, 0, 0, 0);
    }
}

void PluginEditor::toggleAdvancedPanel()
{
    showAdvancedPanel = !showAdvancedPanel;

    // Update button appearance
    advancedButton.setToggleState (showAdvancedPanel, juce::dontSendNotification);

    // Show/hide secondary controls
    filterEnvAttackSlider.setVisible (showAdvancedPanel);
    filterEnvAttackLabel.setVisible (showAdvancedPanel);
    filterEnvDecaySlider.setVisible (showAdvancedPanel);
    filterEnvDecayLabel.setVisible (showAdvancedPanel);
    filterEnvSustainSlider.setVisible (showAdvancedPanel);
    filterEnvSustainLabel.setVisible (showAdvancedPanel);
    filterEnvReleaseSlider.setVisible (showAdvancedPanel);
    filterEnvReleaseLabel.setVisible (showAdvancedPanel);
    filterEnvAmountSlider.setVisible (showAdvancedPanel);
    filterEnvAmountLabel.setVisible (showAdvancedPanel);

    ampAttackSlider.setVisible (showAdvancedPanel);
    ampAttackLabel.setVisible (showAdvancedPanel);
    ampDecaySlider.setVisible (showAdvancedPanel);
    ampDecayLabel.setVisible (showAdvancedPanel);
    ampSustainSlider.setVisible (showAdvancedPanel);
    ampSustainLabel.setVisible (showAdvancedPanel);
    ampReleaseSlider.setVisible (showAdvancedPanel);
    ampReleaseLabel.setVisible (showAdvancedPanel);

    lfoRateSlider.setVisible (showAdvancedPanel);
    lfoRateLabel.setVisible (showAdvancedPanel);
    lfoAmountSlider.setVisible (showAdvancedPanel);
    lfoAmountLabel.setVisible (showAdvancedPanel);

    velToFilterSlider.setVisible (showAdvancedPanel);
    velToFilterLabel.setVisible (showAdvancedPanel);
    velToAmpSlider.setVisible (showAdvancedPanel);
    velToAmpLabel.setVisible (showAdvancedPanel);

    filterKeyTrackSlider.setVisible (showAdvancedPanel);
    filterKeyTrackLabel.setVisible (showAdvancedPanel);

    unisonVoicesSlider.setVisible (showAdvancedPanel);
    unisonVoicesLabel.setVisible (showAdvancedPanel);

    subOctaveCombo.setVisible (showAdvancedPanel);
    subOctaveLabel.setVisible (showAdvancedPanel);

    // Resize window
    if (showAdvancedPanel)
        setSize (1200, 500);  // Main (220) + Advanced panel (280) - more space for knobs
    else
        setSize (1200, 220);  // Main controls with visualizer

    repaint();
    resized();
}
