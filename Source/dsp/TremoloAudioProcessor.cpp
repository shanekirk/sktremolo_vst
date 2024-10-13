// MIT License
//
// Copyright(C), 2024, Shane Kirk

#include "dsp/TremoloAudioProcessor.h"
#include "ui/TremoloAudioProcessorEditor.h"

/**
 * @since 2024 Oct 1
 */
TremoloAudioProcessor::TremoloAudioProcessor()
    : AudioProcessor(BusesProperties()
                       .withInput("Input", juce::AudioChannelSet::stereo(), true)
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      m_audioParams(this)
{
    static_assert(std::atomic<int>::is_always_lock_free,
        "Cannot perform atomic operations on target platform without locking.");
    m_audioParams.addListener(this);
}

/**
 * @since 2024 Oct 7
 */
TremoloAudioProcessor::~TremoloAudioProcessor()
{
    m_audioParams.removeListener(this);
}

/**
 * @since 2024 Oct 1
 */
void TremoloAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    // We update ALL of our wave generators so the user can toggle between them
    // all cleanly at runtime.
    for (auto &spOscillator : m_oscillators)
    {
        spOscillator->setSampleRate(sampleRate);
        spOscillator->setFrequency(*(m_audioParams.m_pSpeedParamInHz));
        spOscillator->reset();
    }
}

/**
 * @since 2024 Oct 1
 */
bool TremoloAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

/**
 * @since 2024 Oct 1
 */
void TremoloAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &/*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // The magic
    for (auto sampleIndex = 0; sampleIndex < buffer.getNumSamples(); ++sampleIndex)
    {
        float* const* channelPointers = buffer.getArrayOfWritePointers();

        double tremoloValue = calculateTremoloEffectValue();
        for (auto channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* pChannelValue = channelPointers[channel] + sampleIndex;
            double newValue = static_cast<double>(*pChannelValue) * tremoloValue;
            *pChannelValue = static_cast<float>(newValue);
        }
    }
}

/**
 * @since 2024 Oct 1
 */
juce::AudioProcessorEditor *TremoloAudioProcessor::createEditor()
{
    return new TremoloAudioProcessorEditor(*this, m_audioParams);
}

/**
 * @since 2024 Oct 7
 */
void TremoloAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    switch (parameterIndex)
    {
        case 0:
        {
            float speed = m_audioParams.m_pSpeedParamInHz->convertFrom0to1(newValue);
            // We update ALL of our wave generators so the user can toggle between them
            // all cleanly at runtime.
            for (auto &spOscillator : m_oscillators)
            {
                spOscillator->setFrequency(speed);
                spOscillator->setFrequency(*m_audioParams.m_pSpeedParamInHz);
                spOscillator->reset();
            }
            break;
        }
        case 1: break; // Depth changes are picked up automatically as part of the stream processing.
        case 2: break; // Gain changes are picked up automatically as part of the stream processing.
        case 3: break; // Oscillator changes are picked up automatically as part of the stream processing.
    }
}

/**
 * @since 2024 Oct 11
 */
double TremoloAudioProcessor::calculateTremoloEffectValue() const
{
    double effectValue = 0.0;
    int oscillatorIndex = *m_audioParams.m_pOscillatorType;
    for (size_t i = 0; i < m_oscillators.size(); ++i)
    {
        if (i == oscillatorIndex)
            effectValue = m_oscillators[i]->nextValue();
        else
            m_oscillators[i]->skipNextValue();
    }

    // We currently have a value between -1.0 and 1.0. Let's convert it to
    // a percentage it's between 0.0 and 1.0.
    effectValue = (effectValue + 1.0) * 0.5;

    // Now let's apply the depth value.
    if (*m_audioParams.m_pDepthParamInPercent < 1.0)
        effectValue = effectValue * (*m_audioParams.m_pDepthParamInPercent) + (1.0 - *m_audioParams.m_pDepthParamInPercent);

    // Now finally the user provided gain value.
    effectValue *= *m_audioParams.m_pGainParamInPercent;
    return effectValue;
}
