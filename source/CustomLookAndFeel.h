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
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10.0f);
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (6.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;
        auto center = bounds.getCentre();

        // Drop shadow for depth
        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.fillEllipse (bounds.translated (0.0f, 2.0f).reduced (-2.0f));

        // Background circle with gradient
        juce::ColourGradient knobGradient (
            juce::Colour (0xff2f2f2f), center.x, bounds.getY(),
            juce::Colour (0xff1a1a1a), center.x, bounds.getBottom(),
            false);
        g.setGradientFill (knobGradient);
        g.fillEllipse (bounds);

        // Inner shadow ring
        g.setColour (juce::Colours::black.withAlpha (0.5f));
        g.drawEllipse (bounds.reduced (2.0f), 1.5f);

        // Tick marks around the knob
        auto numTicks = 11;
        for (int i = 0; i < numTicks; ++i)
        {
            auto angle = rotaryStartAngle + (i / (float)(numTicks - 1)) * (rotaryEndAngle - rotaryStartAngle);
            auto tickRadius = radius + 5.0f;
            auto tickLength = 4.0f;

            juce::Point<float> tickStart (center.x + std::sin (angle) * tickRadius,
                                         center.y - std::cos (angle) * tickRadius);
            juce::Point<float> tickEnd (center.x + std::sin (angle) * (tickRadius + tickLength),
                                       center.y - std::cos (angle) * (tickRadius + tickLength));

            // Highlight current tick position
            bool isNearValue = std::abs (angle - toAngle) < 0.15f;
            g.setColour (isNearValue ? juce::Colour (0xff888888) : juce::Colour (0xff444444));
            g.drawLine (tickStart.x, tickStart.y, tickEnd.x, tickEnd.y, isNearValue ? 2.5f : 1.5f);
        }

        // Value arc with glow effect
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                               bounds.getCentreY(),
                               arcRadius,
                               arcRadius,
                               0.0f,
                               rotaryStartAngle,
                               toAngle,
                               true);

        // Outer glow
        g.setColour (findColour (juce::Slider::rotarySliderFillColourId).withAlpha (0.3f));
        g.strokePath (valueArc, juce::PathStrokeType (lineW + 4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Main arc
        g.setColour (findColour (juce::Slider::rotarySliderFillColourId));
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Outline circle with highlight
        g.setColour (juce::Colour (0xff555555));
        g.drawEllipse (bounds, 1.5f);

        // Pointer with shadow
        juce::Path pointer;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = lineW * 0.7f;
        pointer.addRectangle (-pointerThickness * 0.5f, -radius + 8.0f, pointerThickness, pointerLength);

        // Pointer shadow
        g.setColour (juce::Colours::black.withAlpha (0.5f));
        g.fillPath (pointer, juce::AffineTransform::rotation (toAngle).translated (center.x + 0.5f, center.y + 0.5f));

        // Pointer
        g.setColour (findColour (juce::Slider::rotarySliderFillColourId));
        g.fillPath (pointer, juce::AffineTransform::rotation (toAngle).translated (center.x, center.y));

        // Center cap with gradient
        juce::ColourGradient capGradient (
            juce::Colour (0xff252525), center.x, center.y - 6.0f,
            juce::Colour (0xff0f0f0f), center.x, center.y + 6.0f,
            false);
        g.setGradientFill (capGradient);
        g.fillEllipse (center.x - 6.0f, center.y - 6.0f, 12.0f, 12.0f);

        // Cap highlight
        g.setColour (juce::Colour (0xff444444));
        g.drawEllipse (center.x - 6.0f, center.y - 6.0f, 12.0f, 12.0f, 1.0f);
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
