// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

#include "Oscillator.h"
#include <atomic>

/**
 * Triangle wave oscillator that produces values in the range of -1.0 to 1.0.
 * Clients are free to dynamically adjust the frequency, sample rate, and
 * phase.
 */
class TriangleWaveOscillator : public Oscillator
{
public:

    /**
     * Constructor.
     */
    TriangleWaveOscillator() :
        m_sampleRate(0.0),
        m_frequency(0.0),
        m_phaseDelta(0.0),
        m_currentPhase(0.0)
    {
        static_assert(std::atomic<double>::is_always_lock_free,
            "Cannot perform atomic operations on target platform without locking.");
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
        recalculatePhaseDelta();
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
        recalculatePhaseDelta();
    }

    /**
     * Returns the next value as calculated by the oscillator. Values will be
     * between -1.0 <= value <= 1.0.
     */
    double nextValue() override
    {
        const double amplitude = 1.0;
        const double phaseDelta = m_phaseDelta;
        double phase = m_currentPhase;

        double sampleValue = 0.0;
        if (phase < 0.5)
            sampleValue = 4.0 * amplitude * phase - amplitude;
        else
            sampleValue = 4.0 * amplitude * (1.0 - phase) - amplitude;

        phase = phase + phaseDelta;
        if (phase > 1.0)
            phase = 1.0 - phase;

        m_currentPhase = phase;

        return sampleValue;
    }

    /**
     * Advances the oscillation phase such that the next value is effectively
     * skipped without performing any calculations of what the value should be.
     */
    void skipNextValue() override
    {
        const double phaseDelta = m_phaseDelta;
        double phase = m_currentPhase;
        phase = phase + phaseDelta;
        if (phase > 1.0)
            phase = 1.0 - phase;

        m_currentPhase = phase;
    }

    /**
     * Resets the oscillation phase back to starting position.
     */
    void reset() override { m_currentPhase = 0.0; }

private:

    /**
     * Recalculates the change in phase that occurs for every value calculation.
     */
    void recalculatePhaseDelta()
    {
        double phaseDelta = 0.0;
        double sampleRate = m_sampleRate;
        double frequency = m_frequency;

        if ((sampleRate == 0.00) || (sampleRate == -0.00))
            return;

        phaseDelta = frequency / sampleRate;
        m_phaseDelta = phaseDelta;
    }

    std::atomic<double> m_sampleRate;   //!< The audio stream sample rate as set by the client.
    std::atomic<double> m_frequency;    //!< The oscillation frequency as set by the client.
    std::atomic<double> m_phaseDelta;   //!< The calculated change in phase that occurs with every call to nextValue() or skipNextValue();
    std::atomic<double> m_currentPhase; //!< Where the phase currently is.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriangleWaveOscillator)
};
