#include "test_random.h"

namespace random_generator {
    template <std::integral T>
    [[nodiscard]] T get_uniform_random(T left, T right) noexcept {
        std::uniform_int_distribution<T> distribution(left, right);
        return distribution(generator);
    }

    template <std::floating_point T>
    [[nodiscard]] T get_uniform_random(T left, T right) noexcept {
        std::uniform_real_distribution<T> distribution(left, right);
        return distribution(generator);
    }

    template <std::floating_point T>
    [[nodiscard]] T get_normal_distribution(T mean, T stddev) {
        std::normal_distribution<T> distribution(mean, stddev);
        return distribution(generator);
    }
}