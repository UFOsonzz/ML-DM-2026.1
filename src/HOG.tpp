#include "include/image.h"
#include <numbers>
#include <span>
#include <functional>
#include <iterator>
#include <cmath>

class HOG {
public: 
    template <std::size_t N, std::size_t M, std::size_t BIN_SIZE>
    [[nodiscard]] constexpr auto run_hog(const image::RGBImage<N, M>& src) noexcept {
        const auto                grey_image = image::convert_rgb_to_grey_img<N, M>(src);
        const auto                grad_image = convert_grey_to_gradient_img<N, M>(grey_image);
        const auto                histogram  = convert_image_to_histogram<N, M, BIN_SIZE>(grad_image);
        return block_norm(histogram);
    }

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
    [[nodiscard]] constexpr GradientImage<N, M> convert_grey_to_gradient_img(const image::GreyImage<N, M>& src) noexcept {
        GradientImage<N, M> return_grad_image{};
        for (std::size_t row : std::views::iota(0uz, N)) {
            for (std::size_t col : std::views::iota(0uz, M)) {
                image::GreyPixel cur_pixel{src[row][col]};
                image::GreyPixel up_pixel{(row == 0 ? cur_pixel : src[row - 1][col])};
                image::GreyPixel right_pixel{(col == M - 1 ? cur_pixel : src[row][col + 1])};
                
                image::GreyPixel gx{static_cast<image::GreyPixel>(up_pixel - cur_pixel)};
                image::GreyPixel gy{static_cast<image::GreyPixel>(right_pixel - cur_pixel)};
                float mag = std::hypot(gx, gy);
                float ang = std::atan2(gy, gx) * 180 / std::numbers::pi;
                return_grad_image[row][col] = {.mag_ = static_cast<Magnitude>(mag), 
                                               .ang_ = static_cast<Angle>(ang)};
            }
        }

        return return_grad_image;
    }

    template <std::size_t BIN_SIZE>
    using VotingBin = std::array<GradientVec, BIN_SIZE>;

    template <std::size_t HIST_N, std::size_t HIST_M, std::size_t BIN_SIZE>
    using Histogram = std::array<std::array<VotingBin<BIN_SIZE>, HIST_M>, HIST_N>;

    template <std::size_t N, std::size_t M, std::size_t BIN_SIZE>
    [[nodiscard]] constexpr auto convert_image_to_histogram(const GradientImage<N, M>& src) noexcept {
        static constexpr std::size_t GRID_SIZE = 8; // GRID_SIZE X GRID_SIZE subgrid

        static constexpr VotingBin<BIN_SIZE> INIT_BIN = []() constexpr {
            VotingBin<BIN_SIZE> return_bin{};
            auto bin_view = std::views::iota(0, 180) 
                          | std::views::transform([](std::int16_t x) {
                            return SAFE_CAST(std::float16_t, x);
                          }) 
                          | std::views::transform([](std::float16_t ang) -> GradientVec {
                            return GradientVec{0, ang};
                          })
                          | std::views::stride(180.0 / BIN_SIZE);

            std::ranges::copy(bin_view, return_bin.begin());
            return return_bin;
        }();

        static constexpr std::size_t HIST_N = N / GRID_SIZE;
        static constexpr std::size_t HIST_M = M / GRID_SIZE;
        static constexpr std::size_t GRID_NUM = HIST_N * HIST_M;

        Histogram<HIST_N, HIST_M, BIN_SIZE> return_histogram = [] { // no param
            std::array<VotingBin<BIN_SIZE>, HIST_M> init_row{};
            init_row.fill(INIT_BIN);
            Histogram<HIST_N, HIST_M, BIN_SIZE> return_histogram{};
            return_histogram.fill(init_row);  
            return return_histogram;
        }();
        
        // for (std::size_t i{0uz}; i < N; i += GRID_SIZE) {
        //     for (std::size_t j{0uz}; j < M; j += GRID_SIZE) {
        //         VotingBin<BIN_SIZE> cur_bin = INIT_BIN;
        //         // inside a subgrid
        //         for (std::size_t s_i{i}; )
        //     }
        // } cache inefficiency

        for (std::size_t i : std::views::iota(0uz, N)) {
            for (std::size_t j : std::views::iota(0uz, M)) {
                // which grid this belong to
                auto& cur_bin = return_histogram[i / GRID_SIZE][j / GRID_SIZE];
                
                auto closest_match = std::ranges::lower_bound(cur_bin, src[i][j].ang_, {}, &GradientVec::ang_);

                bool is_end{closest_match == std::ranges::end(cur_bin)};

                if (!is_end && closest_match->ang_ == src[i][j].ang_) {
                    closest_match->mag_ += src[i][j].mag_;
                } else {
                    const Magnitude half_contribute = src[i][j].mag_ / 2.0f16;
                    if (!is_end) {
                        std::next(closest_match)->mag_ += half_contribute;
                    }

                    closest_match->mag_ += half_contribute;
                }
            }
        } 

        return return_histogram;
    }

    template <std::size_t F_N>
    using FlatHistogram = std::array<Magnitude, F_N>;

    template <std::size_t HIST_N, std::size_t HIST_M, std::size_t BIN_SIZE> 
    [[nodiscard]] constexpr auto block_norm(const Histogram<HIST_N, HIST_M, BIN_SIZE>& src) noexcept {
        // this returns flattened array of normed histogram
        // consider each 2 x 2 block
        constexpr std::size_t BLOCK_SIZE = BIN_SIZE * 4uz;
        constexpr std::size_t FLAT_SIZE = (HIST_N - 1) * (HIST_M - 1) * BLOCK_SIZE;
        FlatHistogram<FLAT_SIZE> return_flat_histogram{};
        
        std::size_t cur_flat_idx{};
        for (std::size_t i : std::views::iota(0uz, HIST_N - 1)) {
            for (std::size_t j : std::views::iota(0uz, HIST_M - 1)) {
                
                std::array<std::span<const GradientVec>, 4uz> join_spans = {src[i][j], src[i][j + 1], 
                                                                            src[i + 1][j], src[i + 1][j + 1]};
                auto joined_view = join_spans 
                                 | std::views::join 
                                 | std::views::transform(&GradientVec::mag_);
                
                const Magnitude L2_NORM = std::sqrt(
                    1e-6f32 + std::ranges::fold_left(
                        joined_view | std::views::transform([](const Magnitude& x) {
                            return x * x;
                        }),
                        Magnitude{0},
                        std::plus{}
                    )     
                );

                std::ranges::transform(
                    joined_view, 
                    return_flat_histogram.begin() + cur_flat_idx,
                    [L2_NORM](const Magnitude& x) { return x / L2_NORM; }
                );
                
                cur_flat_idx += BLOCK_SIZE;
            }        
        }
            
        return return_flat_histogram;
    }

    HOG() = default;
    ~HOG() = default;
    
};