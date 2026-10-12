#include <cmath>
#pragma once
class Lfo
{
private:
    double sampleRate = 44100.0;
    float frequency = 0.0f;
    float phaseIncrement = 0.0f;
    float phase = 0.0f;

    void updatePhaseIncrement()
    {
        phaseIncrement = frequency / static_cast<float>(sampleRate);
    }

public:
    void prepare(double sr, float f)
    {
        sampleRate = sr;
        frequency = f;
        updatePhaseIncrement();
    }

    void setFrequency(float f)
    {
        frequency = f;
        updatePhaseIncrement();
    }

    float nextSample()
    {
        const float out = 4.0f * std::abs (phase - 0.5f) - 1.0f;

        phase += phaseIncrement;
        if (phase >= 1.0f)
        {
            phase -= 1.0f;
        }

        return out;
    }

    void process(float* buffer, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            buffer[i] = nextSample();
        }
    }
};
