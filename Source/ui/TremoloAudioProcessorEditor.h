// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include "ui/IconButton.h"
#include "ui/Knob.h"
#include "common/AudioParams.h"

#include <JuceHeader.h>

/**
 * The class responsible for providing the plugin UI. Instances of this
 * class are created by the plugin's AudioProcessor.
 */
class TremoloAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::AudioProcessorParameter::Listener
{
public:

    /**
     * Constructor.
     *
     * @param audioProcessor I don't like this circular reference. But I'm not sure how to avoid it since the base class requires it.
     * @param audioParams The parameters the UI controls will be manipulating.
     */
    TremoloAudioProcessorEditor(juce::AudioProcessor &audioProcessor,
        AudioParameters &audioParams);

    /**
     * Destructor.
     */
    ~TremoloAudioProcessorEditor();

    /**
     * Draws the window content.
     *
     * @param g The graphics context use for drawing.
     */
    void paint(juce::Graphics &g) override;

    /**
     * Called when this component's size has been changed. Child components
     * should be resized and repositioned appropriately.
     */
    void resized() override;

    /*************************************************************************
     AudioProcessorParameter::Listener overrides.
     *************************************************************************/

     /**
      * Receives a callback when a parameter has been changed.
      */
    void parameterValueChanged(int parameterIndex, float newValue) override;

    /**
     * Indicates that a parameter change gesture has started.
     *
     * Not currently supported.
     */
    void parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/) override {}

private:

    /**
     * Creates a knob associated with the given parameter.
     */
    static std::unique_ptr<Knob> createKnob(juce::AudioParameterFloat &param);

    /**
     * Creates a label associated with a knob.
     */
    static std::unique_ptr<juce::Label> createKnobLabel(juce::String text, Knob &knob);

    /**
     * Create a wave form button of a given type.
     */
    static std::unique_ptr<IconButton> createWaveFormButton(int waveFormId, juce::AudioParameterChoice &param);

    /**
     * Creates the "About" button that's displayed in the top right corner.
     */
    static std::unique_ptr<juce::DrawableButton> createAboutButton(juce::AudioProcessorEditor *pParent, const juce::Colour &fgColor);

    /**
     * Creates the "SKTremolo" title drawable.
     */
    static std::unique_ptr<juce::Drawable> createTitleDrawable(const juce::Colour &fgColor);

    std::unique_ptr<juce::Label> m_spSpeedLabel; //!< The "Speed" label.
    std::unique_ptr<juce::Label> m_spDepthLabel; //!< The "Depth" label.
    std::unique_ptr<juce::Label> m_spGainLabel;  //!< The "Gain" label.

    std::unique_ptr<Knob> m_spSpeedKnob; //!< Used to control the tremolo speed parameter.
    std::unique_ptr<Knob> m_spDepthKnob; //!< Used to control the tremolo depth parameter.
    std::unique_ptr<Knob> m_spGainKnob;  //!< Used to control the tremolo gain parameter.

    std::unique_ptr<juce::DrawableButton> m_spAboutButton; //!< Displays version, copyright, etc.
    std::unique_ptr<juce::Drawable> m_spTitleDrawable; //!< The "SK Tremolo" graphic.

    std::unique_ptr<IconButton> m_spSineButton;     //!< Allows user to enable a sine wave volume envelope.
    std::unique_ptr<IconButton> m_spSquareButton;   //!< Allows user to enable a square wave volume envelope.
    std::unique_ptr<IconButton> m_spTriangleButton; //!< Allows user to enable a triangle wave volume envelope.

    const juce::Rectangle<int> m_titleRect { 48, 0, 200, 65 }; //!< The region that m_spTitleGraphic occupies. We cache this to use as a clipping region for our border.

    constexpr static const int RADIO_BUTTON_GROUP_ID = 1; //! Button group to use for all radio buttons.

    const juce::Colour backgroundGradientTop{ 19, 108, 134 };
    const juce::Colour backgroundGradientBottom{ 5, 63, 90 };
    const juce::Colour foregroundColor{ 227, 227, 221 };

    AudioParameters &m_audioParams; //!< The audio params the controls will be manipulating.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoloAudioProcessorEditor)
};
