#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class ThiccLogoComponent : public juce::Component
{
public:
    ThiccLogoComponent()
    {
        setSize (200, 140);  // Wider for logo
        loadCustomLogo();
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // If custom logo is loaded, draw it
        if (customLogo.isValid())
        {
            // Draw the custom logo, maintaining aspect ratio
            g.drawImage (customLogo, bounds,
                        juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize);
        }
        else
        {
            // Fallback: Draw text-based logo
            g.setFont (juce::Font (32.0f, juce::Font::bold));
            g.setColour (juce::Colour (0xffE0E0E0));
            g.drawText ("THICC", bounds.removeFromTop (50), juce::Justification::centred);

            g.setFont (juce::Font (28.0f, juce::Font::plain));
            g.setColour (juce::Colour (0xffE0E0E0));
            g.drawText ("BASS", bounds.removeFromTop (40), juce::Justification::centred);

            // Draw diagonal stripe pattern below
            auto stripeArea = bounds.reduced (10.0f, 5.0f);

            // Red stripes (left side)
            for (int i = 0; i < 3; ++i)
            {
                float x = stripeArea.getX() + (i * 12.0f);
                float width = 8.0f;
                juce::Path stripe;
                stripe.addRectangle (x, stripeArea.getY(), width, stripeArea.getHeight());
                g.setColour (juce::Colour (0xffFF3333));
                g.fillPath (stripe);
            }

            // Yellow stripes (right side)
            for (int i = 0; i < 3; ++i)
            {
                float x = stripeArea.getX() + 40.0f + (i * 12.0f);
                float width = 8.0f;
                juce::Path stripe;
                stripe.addRectangle (x, stripeArea.getY(), width, stripeArea.getHeight());
                g.setColour (juce::Colour (0xffFFCC00));
                g.fillPath (stripe);
            }
        }
    }

private:
    void loadCustomLogo()
    {
        // Try to load custom logo from BinaryData
        // thicc-bass-logo.png becomes thiccbasslogo_png in BinaryData
        int dataSize = 0;
        const char* data = BinaryData::getNamedResource ("thiccbasslogo_png", dataSize);

        if (data != nullptr && dataSize > 0)
        {
            customLogo = juce::ImageCache::getFromMemory (data, dataSize);
        }
    }

    juce::Image customLogo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThiccLogoComponent)
};
