// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include "Oscillator.h"
#include <juce_core/juce_core.h>
#include <atomic>

/**
 * Sine wave oscillator that produces values in the range of -1.0 to 1.0.
 * Clients are free to dynamically adjust the frequency, sample rate, and
 * phase.
 */
class SineWaveOscillator : public Oscillator
{
public:

    /**
     * Constructor.
     */
    SineWaveOscillator() :
        m_angleDelta(0.0),
        m_sampleRate(0.0),
        m_frequency(0.0),
        m_currPhase(0.0)
    {
        static_assert(std::atomic<double>::is_always_lock_free,
            "Cannot perform atomic operations on target platform without locking.");

        reset();
    }

    /**
     * Sets the audio stream sample rate.
     *
     * Note that if the sample rate is set to zero, nextValue() will always return
     * zero.
     *
     * @param sampleRate The new audio stream sample rate to be used by the oscillator.
     */
    void setSampleRate(double sampleRate) override
    {
        m_sampleRate = sampleRate;
        recalculateAngleDelta();
    }

    /**
     * Sets the oscillation frequency.
     *
     * Note that if the frequency is set to zero, nextValue() will always return
     * zero.
     *
     * @param frequency Sets the frequency to be used by the oscillator.
     */
    void setFrequency(double frequency) override
    {
        m_frequency = frequency;
        recalculateAngleDelta();
    }

    /**
     * Returns the next value as calculated by the oscillator. Values will be
     * between -1.0 <= value <= 1.0.
     */
    double nextValue() override
    {
        double currPhase = m_currPhase;
        const double angleDelta = m_angleDelta;

        double sample = std::sin(currPhase);
        currPhase = angleDelta + currPhase;
        if (currPhase > juce::MathConstants<double>::twoPi)
            currPhase = currPhase - juce::MathConstants<double>::twoPi;

        m_currPhase = currPhase;
        return sample;
    }

    /**
     * Advances the oscillation phase such that the next value is effectively
     * skipped without performing any calculations of what the value should be.
     */
    void skipNextValue() override
    {
        double currPhase = m_currPhase;
        const double angleDelta = m_angleDelta;

        currPhase = angleDelta + currPhase;
        if (currPhase > juce::MathConstants<double>::twoPi)
            currPhase = currPhase - juce::MathConstants<double>::twoPi;

        m_currPhase = currPhase;
    }

    /**
     * Resets the oscillation phase back to starting position.
     */
    void reset() override
    {
        m_currPhase = 0.0;
        // This let's us ensure the since wave starts at 0 when we squeeze it between 0 and 1.0
        shiftPhaseInRadians(juce::MathConstants<double>::pi * 1.5);
    }

private:

    /**
     * Shifts the phase of the sine wave oscillation by the specified amount
     * in radians.
     *
     * @param radians The number of radians to advance the tracked angle.
     */
    void shiftPhaseInRadians(double radians)
    {
        double currPhase = m_currPhase;

        currPhase = currPhase + radians;
        if (currPhase > juce::MathConstants<double>::twoPi)
            currPhase = currPhase - juce::MathConstants<double>::twoPi;

        m_currPhase = currPhase;
    }

    /**
     * Recalculates the change in angle that occurs for every value calculation.
     */
    void recalculateAngleDelta()
    {
        double sampleRate = m_sampleRate;
        double frequency = m_frequency;

        if ((sampleRate == 0.0) || (frequency == 0.0))
            m_angleDelta = 0.0;

        double cyclesPerSample = frequency / sampleRate;
        m_angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    }

    std::atomic<double> m_angleDelta; //!< The current calculated angle delta. This can change when setSampleRate() or setFrequency() are invoked.
    std::atomic<double> m_sampleRate; //!< The audio stream sample rate as set by the client.
    std::atomic<double> m_frequency;  //!< The oscillation frequency as set by the client.
    std::atomic<double> m_currPhase;  //!< The current phase of the sine wave.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineWaveOscillator)
};
