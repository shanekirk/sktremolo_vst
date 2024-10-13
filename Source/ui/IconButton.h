// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include <JuceHeader.h>

/**
 * Simple toggle button that displays an icon.
 */
class IconButton : public juce::ToggleButton
{
public:

    IconButton() {}

    /**
     * Sets the icon displayed on the button.
     */
    void setIcon(std::unique_ptr<juce::Drawable> spIcon) { m_spIcon = std::move(spIcon); }

    /**
     * Invoked whenever the button needs to repaint itself.
     */
    void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        // Ensures the checkbox is painted.
        ToggleButton::paintButton(g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        if (!m_spIcon) return;

        auto bounds = getLocalBounds();
        float xPos = bounds.getX() + 35.0f; // 35.0 is an offet from the checkbox.
        float yPos = (bounds.getHeight() / 2.0f) - (m_spIcon->getHeight() / 2.0f);
        m_spIcon->drawAt(g, xPos, yPos, 1.0f);
    }

private:

    std::unique_ptr<juce::Drawable> m_spIcon;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconButton)
};
