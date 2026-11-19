#pragma once

#include <array>
#include <cmath>
#include <algorithm>
#include <execution>
#include <ranges>
#include <stdfloat>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <type_traits>
#include <print>

#define SAFE_CAST(D, x) static_cast<D>(x)

namespace image {
    using RGBPixel = std::array<std::uint8_t, 3>;
    using GreyPixel = std::float16_t;
    
    template <std::size_t N, std::size_t M>
    using RGBImage = std::array<std::array<RGBPixel, M>, N>;

    template <std::size_t N, std::size_t M>
    using GreyImage = std::array<std::array<GreyPixel, M>, N>;

    template <std::size_t New_N, std::size_t New_M, std::size_t Old_N, std::size_t Old_M>
    [[nodiscard]] RGBImage<New_N, New_M> resize(const RGBImage<Old_N, Old_M>& src);
    
    template <std::size_t N, std::size_t M>
    inline void print_image(const RGBImage<N, M>& image);

    [[nodiscard]] constexpr GreyPixel convert_rgb_to_grey(const RGBPixel& pixel) noexcept;

    template <std::size_t N, std::size_t M>
    [[nodiscard]] GreyImage<N, M> convert_rgb_to_grey_img(const RGBImage<N, M>& src) noexcept;
}

#include "detail/image_processing.tpp"