#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

class WaveformVisualizerComponent : public juce::Component, public juce::Timer
{
public:
    WaveformVisualizerComponent()
    {
        waveformBuffer.fill (0.0f);
        startTimerHz (60);  // 60 FPS for smooth animation
        setSize (400, 80);
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Dark background with subtle gradient
        juce::ColourGradient bgGradient (
            juce::Colour (0xff1a1a1a), bounds.getCentreX(), bounds.getY(),
            juce::Colour (0xff0f0f0f), bounds.getCentreX(), bounds.getBottom(),
            false);
        g.setGradientFill (bgGradient);
        g.fillRoundedRectangle (bounds, 4.0f);

        // Outer border
        g.setColour (juce::Colour (0xff333333));
        g.drawRoundedRectangle (bounds, 4.0f, 1.5f);

        // Grid lines for reference
        g.setColour (juce::Colour (0xff2a2a2a));
        float centerY = bounds.getCentreY();

        // Horizontal center line
        g.drawLine (bounds.getX(), centerY, bounds.getRight(), centerY, 0.5f);

        // Vertical grid lines
        for (int i = 1; i < 8; ++i)
        {
            float x = bounds.getX() + (bounds.getWidth() / 8.0f) * i;
            g.drawLine (x, bounds.getY(), x, bounds.getBottom(), 0.5f);
        }

        // Draw waveform
        juce::Path waveformPath;
        const float width = bounds.getWidth();
        const float height = bounds.getHeight();
        const float centerHeight = height / 2.0f;
        const float amplitude = centerHeight * 0.85f;  // Leave some margin

        bool firstPoint = true;
        for (size_t i = 0; i < waveformBuffer.size(); ++i)
        {
            float x = bounds.getX() + (i / static_cast<float>(waveformBuffer.size() - 1)) * width;
            float y = centerY + (waveformBuffer[i] * amplitude);

            if (firstPoint)
            {
                waveformPath.startNewSubPath (x, y);
                firstPoint = false;
            }
            else
            {
                waveformPath.lineTo (x, y);
            }
        }

        // Glow effect for waveform
        g.setColour (juce::Colour (0xff66CCFF).withAlpha (0.3f));
        g.strokePath (waveformPath, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Main waveform line
        g.setColour (juce::Colour (0xff66CCFF));
        g.strokePath (waveformPath, juce::PathStrokeType (2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Label
        g.setColour (juce::Colour (0xffffcc66).withAlpha (0.8f));
        g.setFont (juce::Font (9.0f, juce::Font::bold));
        g.drawText ("WAVEFORM", bounds.reduced (4.0f).removeFromTop (12), juce::Justification::left);
    }

    void timerCallback() override
    {
        repaint();
    }

    void pushSamples (const float* samples, int numSamples)
    {
        // Downsample incoming audio to fit our buffer
        const int stride = std::max (1, numSamples / static_cast<int>(waveformBuffer.size()));

        // Rotate buffer to make room for new samples
        size_t samplesToAdd = std::min (static_cast<size_t>(numSamples / stride), waveformBuffer.size());

        // Shift existing samples left
        std::rotate (waveformBuffer.begin(),
                    waveformBuffer.begin() + samplesToAdd,
                    waveformBuffer.end());

        // Add new samples at the end
        for (size_t i = 0; i < samplesToAdd; ++i)
        {
            int sampleIndex = static_cast<int>(i) * stride;
            if (sampleIndex < numSamples)
            {
                waveformBuffer[waveformBuffer.size() - samplesToAdd + i] =
                    juce::jlimit (-1.0f, 1.0f, samples[sampleIndex]);
            }
        }
    }

    void reset()
    {
        waveformBuffer.fill (0.0f);
    }

private:
    static constexpr size_t bufferSize = 512;  // Number of points to display
    std::array<float, bufferSize> waveformBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformVisualizerComponent)
};
