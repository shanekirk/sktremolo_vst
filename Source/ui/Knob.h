// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include <JuceHeader.h>

/**
 * A custom rotary knob from the Tremolo UI. Provides a roundish, retro
 * knob vibe.
 */
class Knob : public juce::Slider
{
public:

    /**
     * Constructor.
     */
    Knob();

    /**
     * Invoked whenever the knob should repaint itself.
     */
    void paint(juce::Graphics &g) override;

private:

    const juce::Colour knobOuterGradient1{ 227, 227, 221 };
    const juce::Colour knobOuterGradient2{ 186, 179, 152 };
    const juce::Colour knobDropShadowColor{ 40, 40, 40 };
    const juce::Colour knobTickColor{ 80, 80, 80 };
    const juce::DropShadow knobDropShadow{ knobDropShadowColor, 3, { 4, 5 } };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};
