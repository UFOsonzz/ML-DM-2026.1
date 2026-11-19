#pragma once

#include <random>
#include <concepts>

namespace random_generator {
    inline std::random_device RANDOM_SEED;
    inline std::mt19937 generator{RANDOM_SEED()};

    template <std::integral T>
    [[nodiscard]] T get_uniform_random(T left, T right) noexcept;

    template <std::floating_point T>
    [[nodiscard]] T get_uniform_random(T left, T right) noexcept;

    template <std::floating_point T>
    [[nodiscard]] T get_normal_distribution_random(T mean, T stddev) noexcept;
}