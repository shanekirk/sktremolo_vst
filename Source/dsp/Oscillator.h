// MIT License
//
// Copyright(C), 2024, Shane Kirk

#pragma once

/**
 * Interface for all oscillator types.
 */
class Oscillator
{
public:

    //! Destructor.
    virtual ~Oscillator() {}

    /**
     * Sets the audio stream sample rate.
     *
     * Note that if the sample rate is set to zero, nextValue() will always return
     * zero. Implementations should ensure thread-safety.
     *
     * @param sampleRate The new audio stream sample rate to be used by the oscillator.
     */
    virtual void setSampleRate(double sampleRate) = 0;

    /**
     * Sets the oscillation frequency.
     *
     * Note that if the frequency is set to zero, nextValue() will always return
     * zero. Implementations should ensure thread-safety.
     *
     * @param frequency Sets the frequency to be used by the oscillator.
     */
    virtual void setFrequency(double frequency) = 0;

    /**
     * Returns the next value as calculated by the oscillator. Values will be
     * between -1.0 <= value <= 1.0.
     *
     * Implementations should ensure thread-safety.
     */
    virtual double nextValue() = 0;

    /**
     * Advances the oscillation phase such that the next value is effectively
     * skipped without performing any calculations of what the value should be.
     *
     * Implementations should ensure thread-safety.
     */
    virtual void skipNextValue() = 0;

    /**
     * Resets the oscillation phase back to starting position.
     *
     * Implementations should ensure thread-safety.
     */
    virtual void reset() = 0;
};
