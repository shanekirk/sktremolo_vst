// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include "dsp/SineWaveOscillator.h"
#include "dsp/TriangleWaveOscillator.h"
#include "dsp/SquareWaveOscillator.h"
#include "common/AudioParams.h"

#include <JuceHeader.h>

/**
 * The workhorse of the plugin. Implements the Tremolo effect.
 *
 * @since 2024 Oct 7
 */
class TremoloAudioProcessor : public juce::AudioProcessor,
                              public juce::AudioProcessorParameter::Listener
{
public:

    /**
     * Constructor.
     */
    TremoloAudioProcessor();

    /**
     * Destructor.
     */
    ~TremoloAudioProcessor();

    /**
     * Gives the plugin the opportunity to do things it wouldn't ordinarily
     * be able to do during the actual audio processing, such as allocations.
     */
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    /**
     * The counterpart to prepareToPlay(). It's called after playback is
     * finished. It gives the plugin the chance to do any housekeeping.
     */
    void releaseResources() override {}

    /**
     * Lets the framework/host know what sort of bus layouts we support.
     * We only support mono and stereo layouts, and the layouts must be
     * consistent such that the input layout should match that of the
     * output layout.
     */
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    /**
     * Processes the next block of audio. This is where all of the interesting
     * things happen.
     */
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;

    /**
     * Creates an instance of the editor UI. The JUCE framework takes ownership
     * of the object and deallocates it at the appropriate time.
     *
     * The editor UI MUST be created with new and must never be held on to
     * as the framework can deallocate it without warning.
     */
    juce::AudioProcessorEditor *createEditor() override;

    /**
     * Allows the plugin to save its state to a blob of memory provided by the
     * host.
     */
    void getStateInformation(juce::MemoryBlock &destData) override { m_audioParams.saveToMemoryBlock(destData); }

    /**
     * Allows the plugin to restore state that was previously provided to
     * the host.
     */
    void setStateInformation(const void *pData, int sizeInBytes) override { m_audioParams.loadFromBuffer(pData, sizeInBytes); }

    /*************************************************************************
     What follows is a number of functions that return various attributes
     about the plugin that allows the host to discover capabilities.
     *************************************************************************/

    /**
     * Returns true if the plugin provides its own editor GUI, which we do.
     */
    bool hasEditor() const override { return true; }

    /**
     * Returns the name of the plugin as it's shown to the user in the host.
     */
    const juce::String getName() const override { return JucePlugin_Name; }

    /**
     * Instructs the framework/host that we don't accept MIDI messages.
     */
    bool acceptsMidi() const override { return false; }

    /**
     * Instructs the framework/host that we don't produce MIDI messages.
     */
    bool producesMidi() const override { return false; }

    /**
     * Instructs the framework/host that we're not a MIDI effect plugin.
     */
    bool isMidiEffect() const override { return false; }

    /**
     * Returns the tail in seconds produced by the processor. For example,
     * if we were implementing a reverb, this might produce extra data at
     * the end of the stream. We don't have that with a tremolo effect,
     * so no need to worry about it here.
     */
    double getTailLengthSeconds() const override { return 0.0; }

    /**
     * Returns the number of presets. We don't really have any, but we should
     * always report at least 1.
     */
    int getNumPrograms() override { return 1; }

    /**
     * Returns the number of the current present. We don't support presets
     * currently, so we just return 0.
     */
    int getCurrentProgram() override { return 0; }

    /**
     * We don't support presets currently, so this does nothing.
     */
    void setCurrentProgram(int /*index*/) override {}

    /**
     * Returns the name of the specified preset. We don't support presets yet
     * so we always just return an empty string.
     */
    const juce::String getProgramName(int /*index*/) override { return {}; }

    /**
     * Allows the user to change the name of a given preset. But we don't
     * currently support presets, so this does nothing.
     */
    void changeProgramName(int /*index*/, const juce::String &/*newName*/) override {}

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
     * Calculates the next tremolo effect value that should be applied to sample data
     * for all channels.
     */
    double calculateTremoloEffectValue() const;

    //! The supported oscillators.
    std::array<std::unique_ptr<Oscillator>, 3> m_oscillators = 
    { 
        std::make_unique<SineWaveOscillator>(),
        std::make_unique<SquareWaveOscillator>(),
        std::make_unique<TriangleWaveOscillator>()
    };

    AudioParameters m_audioParams;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TremoloAudioProcessor)
};
