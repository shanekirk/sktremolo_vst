// MIT License
//
// Copyright(C), 2024, Shane Kirk

#include "common/AudioParams.h"
#include <stdexcept>

/**
 * @since 2024 Oct 11
 */
AudioParameters::AudioParameters(juce::AudioProcessor *pProcessor)
{
    if (!pProcessor)
        throw std::runtime_error("AudioProcessor cannot be null.");

    // Note that the processor owns these and will automatically deallocate the
    // parameters at the appropriate time.
    pProcessor->addParameter(m_pSpeedParamInHz = new juce::AudioParameterFloat(
        juce::ParameterID("speed"), "Speed", 1.0f, 40.0f, 2.0f));
    pProcessor->addParameter(m_pDepthParamInPercent = new juce::AudioParameterFloat(
        juce::ParameterID("depth"), "Depth", 0.0f, 1.0f, 1.0f));
    pProcessor->addParameter(m_pGainParamInPercent = new juce::AudioParameterFloat(
        juce::ParameterID("gain"), "Gain", 0.0f, 1.0f, 1.0f));
    pProcessor->addParameter(m_pOscillatorType = new juce::AudioParameterChoice(
        juce::ParameterID("oscillator"), "WaveForm Type", { "Sine", "Square", "Triangle" }, 0));
}

/**
 * @since 2024 Oct 11
 */
void AudioParameters::addListener(juce::AudioProcessorParameter::Listener *pListener)
{
    if (!pListener)
        throw std::runtime_error("Listener cannot be null.");

    m_pSpeedParamInHz->addListener(pListener);
    m_pDepthParamInPercent->addListener(pListener);
    m_pGainParamInPercent->addListener(pListener);
    m_pOscillatorType->addListener(pListener);
}

/**
 * @since 2024 Oct 11
 */
void AudioParameters::removeListener(juce::AudioProcessorParameter::Listener *pListener)
{
    if (!pListener)
        throw std::runtime_error("Listener cannot be null.");

    m_pSpeedParamInHz->removeListener(pListener);
    m_pDepthParamInPercent->removeListener(pListener);
    m_pGainParamInPercent->removeListener(pListener);
    m_pOscillatorType->removeListener(pListener);
}

/**
 * @since 2024 Oct 11
 */
void AudioParameters::saveToMemoryBlock(juce::MemoryBlock &destData)
{
    const size_t requiredSize = (sizeof(float) * 3) + sizeof(int);

    if (destData.getSize() < requiredSize)
        destData.setSize(requiredSize);

    unsigned char *pBytes = static_cast<unsigned char *>(destData.getData());

    float speedParamInHz = *m_pSpeedParamInHz;
    memcpy(pBytes, &speedParamInHz, sizeof(float));
    pBytes += sizeof(float);

    float depthParamInPercent = *m_pDepthParamInPercent;
    memcpy(pBytes, &depthParamInPercent, sizeof(float));
    pBytes += sizeof(float);

    float gainParamInPercent = *m_pGainParamInPercent;
    memcpy(pBytes, &gainParamInPercent, sizeof(float));
    pBytes += sizeof(float);

    int oscillatorType = *m_pOscillatorType;
    memcpy(pBytes, &oscillatorType, sizeof(int));
}

/**
 * @since 2024 Oct 11
 */
void AudioParameters::loadFromBuffer(const void *pData, int sizeInBytes)
{
    const size_t requiredSize = (sizeof(float) * 3) + sizeof(int);

    if (sizeInBytes < requiredSize)
        return; // Doesn't have all of our data.

    const unsigned char *pBytes = static_cast<const unsigned char *>(pData);

    float speedParamInHz = 0.0f;
    memcpy(&speedParamInHz, pBytes, sizeof(float));
    pBytes += sizeof(float);

    float depthParamInPercent = 0.0f;
    memcpy(&depthParamInPercent, pBytes, sizeof(float));
    pBytes += sizeof(float);

    float gainParamInPercent = 0.0f;
    memcpy(&gainParamInPercent, pBytes, sizeof(float));
    pBytes += sizeof(float);

    int oscillatorType = 0;
    memcpy(&oscillatorType, pBytes, sizeof(int));

    *m_pSpeedParamInHz = speedParamInHz;
    *m_pDepthParamInPercent = depthParamInPercent;
    *m_pGainParamInPercent = gainParamInPercent;
    *m_pOscillatorType = oscillatorType;
}
