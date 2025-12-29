#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ThiccLogoComponent : public juce::Component
{
public:
    ThiccLogoComponent()
    {
        setSize (140, 140);
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Draw "THICC" text
        g.setFont (juce::Font (32.0f, juce::Font::bold));
        g.setColour (juce::Colour (0xffE0E0E0));
        g.drawText ("THICC", bounds.removeFromTop (50), juce::Justification::centred);

        // Draw "BASS" text
        g.setFont (juce::Font (28.0f, juce::Font::plain));
        g.setColour (juce::Colour (0xffE0E0E0));
        g.drawText ("BASS", bounds.removeFromTop (40), juce::Justification::centred);

        // Draw diagonal stripe pattern below (like "DRIVE" in reference)
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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThiccLogoComponent)
};
