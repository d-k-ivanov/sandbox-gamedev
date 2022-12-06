#pragma once

#include <random>

namespace Math
{
    inline extern constexpr float PI = 3.14159265359f;

    template<typename T>
    T randomNumber(T min, T max) {
        std::random_device randDevice;
        std::mt19937 generator(randDevice());
        if constexpr  (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> distribution(min, max);
            return distribution(generator);
        }
        else
        {
            std::uniform_int_distribution<T> distribution(min, max);
            return distribution(generator);
        }
    }
}
