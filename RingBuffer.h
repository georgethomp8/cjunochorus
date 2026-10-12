#include <array>
#pragma once
class RingBuffer
{
private:
    static constexpr int size = 4096;
    static constexpr int mask = size - 1;
    std::array<float, size> buffer {};
    int writeIndex = 0;

    float clampDelay(float delay) const
    {
        if (delay < 0.0f)
        {
            delay = 0.0f;
        }
        if (delay > size - 2)
        {
            delay = size - 2;
        }
        return delay;
    }

public:
    void clear()
    {
        buffer.fill (0.0f);
        writeIndex = 0;
    }

    void push(float sample)
    {
        buffer[writeIndex] = sample;
        writeIndex = (writeIndex + 1) & mask;
    }

    float read(float delay) const
    {
        const float readPos = writeIndex - 1 + size - clampDelay (delay);
        const int index = static_cast<int>(readPos);
        const float frac = readPos - index;

        const float older = buffer[index & mask];
        const float newer = buffer[(index + 1) & mask];

        return older + frac * (newer - older);
    }
};
