#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ThiccBassLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ThiccBassLookAndFeel()
    {
        // Set color scheme - cartoonish/street art style
        setColour (juce::Slider::thumbColourId, juce::Colour (0xffff3366));      // Hot pink/red
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffff3366));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff000000));
        setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xff000000));
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xffffffff));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

        setColour (juce::Label::textColourId, juce::Colour (0xff000000));
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10.0f);
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto center = bounds.getCentre();

        // CARTOON STYLE: Bold drop shadow (offset down-right)
        g.setColour (juce::Colours::black.withAlpha (0.3f));
        g.fillEllipse (bounds.translated (4.0f, 4.0f));

        // Main knob body - bright vibrant color
        juce::ColourGradient gradient (
            juce::Colour (0xffff6699), center.x - radius * 0.3f, center.y - radius * 0.3f,  // Light pink top-left
            juce::Colour (0xffff0066), center.x + radius * 0.3f, center.y + radius * 0.3f,  // Deep pink bottom-right
            true);
        g.setGradientFill (gradient);
        g.fillEllipse (bounds);

        // CARTOON STYLE: Bold black outline (comic book style)
        g.setColour (juce::Colours::black);
        g.drawEllipse (bounds, 4.0f);

        // Inner highlight for "pop" effect
        auto highlightBounds = bounds.reduced (8.0f);
        g.setColour (juce::Colours::white.withAlpha (0.4f));
        g.fillEllipse (highlightBounds.removeFromTop (highlightBounds.getHeight() * 0.4f));

        // Bold indicator line with outline
        juce::Path indicator;
        auto indicatorLength = radius * 0.6f;
        auto indicatorThickness = 6.0f;

        indicator.addRectangle (-indicatorThickness * 0.5f, -radius * 0.8f, indicatorThickness, indicatorLength);

        // Indicator outline (black)
        g.setColour (juce::Colours::black);
        g.fillPath (indicator, juce::AffineTransform::rotation (toAngle).translated (center.x, center.y));

        // Indicator fill (bright yellow)
        auto innerIndicator = indicator.createPathWithRoundedCorners (2.0f);
        g.setColour (juce::Colour (0xffffdd00));
        g.fillPath (innerIndicator, juce::AffineTransform::rotation (toAngle)
                                                           .scaled (0.7f, 0.9f)
                                                           .translated (center.x, center.y));

        // Center circle with outline
        auto centerRadius = radius * 0.2f;
        g.setColour (juce::Colours::black);
        g.fillEllipse (center.x - centerRadius - 2.0f, center.y - centerRadius - 2.0f,
                       (centerRadius + 2.0f) * 2.0f, (centerRadius + 2.0f) * 2.0f);
        g.setColour (juce::Colour (0xffffdd00));
        g.fillEllipse (center.x - centerRadius, center.y - centerRadius,
                       centerRadius * 2.0f, centerRadius * 2.0f);
    }

    juce::Label* createSliderTextBox (juce::Slider& slider) override
    {
        auto* l = LookAndFeel_V4::createSliderTextBox (slider);
        l->setJustificationType (juce::Justification::centred);
        l->setFont (juce::Font (12.0f, juce::Font::bold));
        l->setColour (juce::Label::textColourId, juce::Colour (0xff000000));  // Black text
        l->setColour (juce::Label::backgroundColourId, juce::Colour (0xffffffdd));  // Light yellow background for contrast
        l->setColour (juce::Label::outlineColourId, juce::Colours::transparentBlack);  // No border
        return l;
    }
};
