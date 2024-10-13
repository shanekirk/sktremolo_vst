// MIT License
//
// Copyright(C), 2024, Shane Kirk

#include "Knob.h"

/**
 * @since 2024 Sept 28
 */
Knob::Knob()
{
    setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
}

/**
 * @since 2024 Sept 28
 */
void Knob::paint(juce::Graphics &g)
{
    auto layout = getLookAndFeel().getSliderLayout(*this);

    auto knobSize = static_cast<float>(std::min(layout.sliderBounds.getWidth(),
        layout.sliderBounds.getHeight()));
    auto knobX = layout.sliderBounds.getCentreX() - (knobSize / 2.0f);
    auto knobY = layout.sliderBounds.getCentreY() - (knobSize / 2.0f);

    // Outer Rect
    juce::Rectangle<float> outerRect(knobX, knobY, knobSize, knobSize);
    outerRect = outerRect.reduced(10.0f, 10.0f);

    juce::Path dropShadowPath;
    dropShadowPath.addEllipse(outerRect);
    knobDropShadow.drawForPath(g, dropShadowPath);

    auto outerGradient = juce::ColourGradient(
        knobOuterGradient1, knobX, knobY,
        knobOuterGradient2, knobX + knobSize, knobY + knobSize, false);
    g.setGradientFill(outerGradient);
    g.fillEllipse(outerRect.toFloat());

    // Inner Rect
    juce::Rectangle<float> innerRect(knobX, knobY, knobSize, knobSize);
    innerRect = innerRect.reduced(20.0f, 20.0f);

    auto innerGradient = juce::ColourGradient(
        knobOuterGradient2, knobX, knobY,
        knobOuterGradient1, knobX + knobSize, knobY + knobSize, false);
    g.setGradientFill(innerGradient);
    g.fillEllipse(innerRect.toFloat());

    // The "tick" part of the knob
    float tickLength = innerRect.getWidth() / 2.0f;
    auto rotaryParams = getRotaryParameters();
    float value = static_cast<float>(getValue() / getMaximum());
    auto toAngle = rotaryParams.startAngleRadians + value * (rotaryParams.endAngleRadians - rotaryParams.startAngleRadians);

    juce::Point<float> tickStart(innerRect.getCentre().getX() + 25 * std::sin(toAngle),
        innerRect.getCentre().getY() - 25 * std::cos(toAngle));
    juce::Point<float> tickEnd(innerRect.getCentre().getX() + (tickLength - 3) * std::sin(toAngle),
        innerRect.getCentre().getY() - (tickLength - 3) * std::cos(toAngle));

    juce::Path tickPath;
    tickPath.startNewSubPath(tickStart);
    tickPath.lineTo(tickEnd);
    g.setColour(knobTickColor);
    auto strokeType = juce::PathStrokeType(3, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    g.strokePath(tickPath, strokeType);
}
