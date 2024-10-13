// MIT License
//
// Copyright(C), 2024, Shane Kirk

#include "dsp/TremoloAudioProcessor.h"

/**
 * The is effectively the entry-point to the plugin. Hosts will invoke this to
 * create a plugin instance.
 *
 * @since 2024 Oct 7
 */
juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter()
{
    return new TremoloAudioProcessor();
}
