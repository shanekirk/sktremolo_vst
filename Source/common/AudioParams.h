// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include <JuceHeader.h>

/**
 * Captures all of the parameters we'll be using in our plugin.
 *
 * @since 2024 Oct 7
 */
struct AudioParameters
{
    juce::AudioParameterFloat *m_pSpeedParamInHz;      //!< How fast the tremolo oscillation occurs.
    juce::AudioParameterFloat *m_pDepthParamInPercent; //!< 100% goes from full gain to zero. 0% sounds like no oscillation at all.
    juce::AudioParameterFloat *m_pGainParamInPercent;  //!< Overall gain adjustment for the samples leaving the plugin.
    juce::AudioParameterChoice *m_pOscillatorType;     //!< What type of tremolo wave to use - sine, square, or triangle.

    /**
     * Constructor.
     */
    explicit AudioParameters(juce::AudioProcessor *pProcessor);

    /**
     * Adds a listener to all parameters.
     */
    void addListener(juce::AudioProcessorParameter::Listener *pListener);

    /**
     * Removes a listener from all parameters.
     */
    void removeListener(juce::AudioProcessorParameter::Listener *pListener);

    /**
     * Saves parameter data to MemoryBlock.
     */
    void saveToMemoryBlock(juce::MemoryBlock &destData);

    /**
     * Loads parameter data from a data buffer.
     */
    void loadFromBuffer(const void *pData, int sizeInBytes);

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioParameters)
};
