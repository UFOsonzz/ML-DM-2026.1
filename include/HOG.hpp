#pragma once

#include "include/image.hpp"
#include <numbers>
#include <span>
#include <functional>
#include <iterator>
#include <cmath>

class HOG {
public: 
    template <std::size_t N, std::size_t M, std::size_t BIN_SIZE>
    [[nodiscard]] constexpr auto run_hog(const image::RGBImage<N, M>& src) noexcept;

    static HOG& get_instance() {
        static HOG instance{};
        return instance;
    }

    HOG(HOG&) = delete;
    HOG& operator=(const HOG&) = delete;
    HOG(HOG&&) = delete;
    HOG& operator=(HOG&&) = delete;

private:
    using Magnitude = std::float32_t;
    using Angle = std::float16_t;
    struct GradientVec {
        Magnitude mag_;
        Angle     ang_;
    };

    template <std::size_t N, std::size_t M>
    using GradientImage = std::array<std::array<GradientVec, M>, N>;

    template <std::size_t N, std::size_t M>
    [[nodiscard]] constexpr GradientImage<N, M> convert_grey_to_gradient_img(const image::GreyImage<N, M>& src) noexcept;

    template <std::size_t BIN_SIZE>
    using VotingBin = std::array<GradientVec, BIN_SIZE>;

    template <std::size_t HIST_N, std::size_t HIST_M, std::size_t BIN_SIZE>
    using Histogram = std::array<std::array<VotingBin<BIN_SIZE>, HIST_M>, HIST_N>;

    template <std::size_t N, std::size_t M, std::size_t BIN_SIZE>
    [[nodiscard]] constexpr auto convert_image_to_histogram(const GradientImage<N, M>& src) noexcept;

    template <std::size_t F_N>
    using FlatHistogram = std::array<Magnitude, F_N>;

    template <std::size_t HIST_N, std::size_t HIST_M, std::size_t BIN_SIZE> 
    [[nodiscard]] constexpr auto block_norm(const Histogram<HIST_N, HIST_M, BIN_SIZE>& src) noexcept;

    HOG() = default;
    ~HOG() = default;
    
};

#include "detail/HOG.tpp"