#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

class OutputMeterComponent : public juce::Component, public juce::Timer
{
public:
    OutputMeterComponent()
    {
        startTimerHz (30);  // 30 FPS refresh rate
        setSize (120, 140);
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Label
        g.setFont (juce::Font (11.0f, juce::Font::bold));
        g.setColour (juce::Colour (0xffffcc66).withAlpha (0.9f));
        g.drawText ("OUTPUT", bounds.removeFromTop (20), juce::Justification::centred);

        // Circular meter area
        auto meterBounds = bounds.reduced (10.0f, 5.0f);
        auto radius = juce::jmin (meterBounds.getWidth(), meterBounds.getHeight()) / 2.0f;
        auto center = meterBounds.getCentre();

        // Outer ring shadow
        g.setColour (juce::Colours::black.withAlpha (0.5f));
        g.fillEllipse (center.x - radius + 2.0f, center.y - radius + 2.0f, radius * 2.0f, radius * 2.0f);

        // Background circle
        juce::ColourGradient bgGradient (
            juce::Colour (0xff1a1a1a), center.x, center.y - radius,
            juce::Colour (0xff0f0f0f), center.x, center.y + radius,
            false);
        g.setGradientFill (bgGradient);
        g.fillEllipse (center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

        // Meter level arc
        float startAngle = juce::MathConstants<float>::pi * 1.25f;  // Bottom-left
        float endAngle = juce::MathConstants<float>::pi * 2.75f;    // Bottom-right (270 degrees)
        float levelAngle = startAngle + (displayLevel * (endAngle - startAngle));

        juce::Path levelArc;
        levelArc.addCentredArc (center.x, center.y,
                               radius - 15.0f, radius - 15.0f,
                               0.0f,
                               startAngle, levelAngle,
                               true);

        // Determine color based on level (blue → yellow → red)
        juce::Colour levelColor;
        if (displayLevel < 0.7f)
            levelColor = juce::Colour (0xff66CCFF);  // Blue (normal)
        else if (displayLevel < 0.9f)
            levelColor = juce::Colour (0xffFFCC00);  // Yellow (warning)
        else
            levelColor = juce::Colour (0xffFF3333);  // Red (clip danger)

        // Glow effect
        g.setColour (levelColor.withAlpha (0.3f));
        g.strokePath (levelArc, juce::PathStrokeType (14.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Main arc
        g.setColour (levelColor);
        g.strokePath (levelArc, juce::PathStrokeType (10.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Peak hold indicator
        if (peakLevel > 0.05f)
        {
            float peakAngle = startAngle + (peakLevel * (endAngle - startAngle));
            juce::Point<float> peakPoint (
                center.x + std::cos (peakAngle) * (radius - 15.0f),
                center.y + std::sin (peakAngle) * (radius - 15.0f)
            );

            g.setColour (juce::Colour (0xffFFCC66));
            g.fillEllipse (peakPoint.x - 4.0f, peakPoint.y - 4.0f, 8.0f, 8.0f);
        }

        // Tick marks
        for (int i = 0; i <= 10; ++i)
        {
            float angle = startAngle + (i / 10.0f) * (endAngle - startAngle);
            float tickRadius1 = radius - 8.0f;
            float tickRadius2 = radius - 3.0f;

            juce::Point<float> tickStart (
                center.x + std::cos (angle) * tickRadius1,
                center.y + std::sin (angle) * tickRadius1
            );
            juce::Point<float> tickEnd (
                center.x + std::cos (angle) * tickRadius2,
                center.y + std::sin (angle) * tickRadius2
            );

            g.setColour (juce::Colour (0xff444444));
            g.drawLine (tickStart.x, tickStart.y, tickEnd.x, tickEnd.y, 1.5f);
        }

        // Center text showing level in dB
        float levelDb = juce::Decibels::gainToDecibels (displayLevel + 0.001f);
        juce::String levelText = levelDb > -60.0f ? juce::String (levelDb, 1) + " dB" : "-inf";

        g.setFont (juce::Font (14.0f, juce::Font::bold));
        g.setColour (juce::Colour (0xffE0E0E0));
        g.drawText (levelText,
                   juce::Rectangle<float> (center.x - 40.0f, center.y - 10.0f, 80.0f, 20.0f),
                   juce::Justification::centred);

        // Outer ring
        g.setColour (juce::Colour (0xff555555));
        g.drawEllipse (center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f, 2.0f);

        // "PHASE" label at bottom (like reference image)
        g.setFont (juce::Font (10.0f, juce::Font::plain));
        g.setColour (juce::Colour (0xff888888));
        g.drawText ("LEVEL", bounds.removeFromBottom (15), juce::Justification::centred);
    }

    void timerCallback() override
    {
        // Professional ballistic meter behavior
        // Fast attack, smooth decay (VU-style ballistics)
        const float decayRate = 0.85f;  // Slower, smoother decay

        // Smooth decay for main level
        displayLevel = displayLevel * decayRate + targetLevel * (1.0f - decayRate);

        // Handle peak hold with timer
        if (peakHoldTime > 0)
        {
            peakHoldTime--;
        }
        else
        {
            // Slow peak decay after hold time expires
            peakLevel *= 0.995f;
        }

        if (displayLevel < 0.001f)
            displayLevel = 0.0f;
        if (peakLevel < 0.001f)
            peakLevel = 0.0f;

        // Decay target level continuously
        targetLevel *= 0.9f;

        repaint();
    }

    void setLevel (float newLevel)
    {
        newLevel = juce::jlimit (0.0f, 1.0f, newLevel);

        // Fast attack - immediately update target if higher than current
        if (newLevel > targetLevel)
            targetLevel = newLevel;

        // Update peak with 2-second hold time (60 frames at 30 FPS)
        if (newLevel > peakLevel)
        {
            peakLevel = newLevel;
            peakHoldTime = 60;  // Hold for 2 seconds
        }
    }

    void reset()
    {
        targetLevel = 0.0f;
        displayLevel = 0.0f;
        peakLevel = 0.0f;
        peakHoldTime = 0;
    }

private:
    float targetLevel = 0.0f;      // Incoming level
    float displayLevel = 0.0f;     // Smoothed display level
    float peakLevel = 0.0f;        // Peak indicator
    int peakHoldTime = 0;          // Peak hold timer (frames)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputMeterComponent)
};
