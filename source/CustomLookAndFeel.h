#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ThiccBassLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ThiccBassLookAndFeel()
    {
        // Set color scheme
        setColour (juce::Slider::thumbColourId, juce::Colour (0xffffcc66));      // Gold/yellow
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffffcc66));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff333333));
        setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff1a1a1a));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

        setColour (juce::Label::textColourId, juce::Colours::white);
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (8.0f);
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (8.0f, radius * 0.15f);
        auto arcRadius = radius - lineW * 0.5f;
        auto center = bounds.getCentre();

        // Larger, cleaner drop shadow
        g.setColour (juce::Colours::black.withAlpha (0.6f));
        g.fillEllipse (bounds.translated (0.0f, 3.0f).reduced (-3.0f));

        // Main knob body - solid color like reference image
        g.setColour (juce::Colour (0xffffcc66));  // Gold color
        g.fillEllipse (bounds);

        // Subtle inner shadow for depth
        g.setColour (juce::Colours::black.withAlpha (0.15f));
        g.drawEllipse (bounds.reduced (2.0f), 2.0f);

        // Tick marks around the knob (cleaner, reference-image style)
        auto numTicks = 11;
        for (int i = 0; i < numTicks; ++i)
        {
            auto angle = rotaryStartAngle + (i / (float)(numTicks - 1)) * (rotaryEndAngle - rotaryStartAngle);
            auto tickRadius = radius + 8.0f;
            auto tickLength = 5.0f;

            juce::Point<float> tickStart (center.x + std::sin (angle) * tickRadius,
                                         center.y - std::cos (angle) * tickRadius);
            juce::Point<float> tickEnd (center.x + std::sin (angle) * (tickRadius + tickLength),
                                       center.y - std::cos (angle) * (tickRadius + tickLength));

            // More prominent tick marks
            g.setColour (juce::Colour (0xffE0E0E0).withAlpha (0.3f));
            g.drawLine (tickStart.x, tickStart.y, tickEnd.x, tickEnd.y, 2.0f);
        }

        // Indicator line (like reference image - single line from center)
        juce::Path indicator;
        auto indicatorLength = radius * 0.4f;
        auto indicatorThickness = 4.0f;

        indicator.addRectangle (-indicatorThickness * 0.5f, -radius * 0.7f, indicatorThickness, indicatorLength);

        // Indicator shadow
        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.fillPath (indicator, juce::AffineTransform::rotation (toAngle).translated (center.x + 1.0f, center.y + 1.0f));

        // Main indicator (dark contrasting line)
        g.setColour (juce::Colour (0xff2a2a2a));
        g.fillPath (indicator, juce::AffineTransform::rotation (toAngle).translated (center.x, center.y));

        // Center dot for depth
        auto centerDotRadius = radius * 0.12f;
        g.setColour (juce::Colour (0xff1a1a1a));
        g.fillEllipse (center.x - centerDotRadius, center.y - centerDotRadius,
                       centerDotRadius * 2.0f, centerDotRadius * 2.0f);

        // Outer ring
        g.setColour (juce::Colour (0xff888888).withAlpha (0.5f));
        g.drawEllipse (bounds, 2.0f);
    }

    juce::Label* createSliderTextBox (juce::Slider& slider) override
    {
        auto* l = LookAndFeel_V4::createSliderTextBox (slider);
        l->setJustificationType (juce::Justification::centred);
        l->setFont (juce::Font (11.0f, juce::Font::bold));
        l->setColour (juce::Label::textColourId, juce::Colour (0xffffcc66));
        l->setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        l->setColour (juce::Label::outlineColourId, juce::Colours::transparentBlack);
        return l;
    }
};
