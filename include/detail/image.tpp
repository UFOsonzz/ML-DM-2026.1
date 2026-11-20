#include "include/image.hpp"

namespace image {
    template <std::size_t New_N, std::size_t New_M, std::size_t Old_N, std::size_t Old_M>
    [[nodiscard]] RGBImage<New_N, New_M> resize(const RGBImage<Old_N, Old_M>& src) {
        RGBImage<New_N, New_M> new_image{};

        // auto flattened_image = new_image | std::views::join;
        // auto resized_image = std::views::iota(std::size_t{0}, New_N * New_M)
        //                     | std::views::transform([&src, New_N, New_M, Old_N, Old_M](size_t idx) {
                            
        //                         std::size_t cor_x = idx / New_M;
        //                         std::size_t cor_y = idx % New_M;
                                
        //                         cor_x = SAFE_CAST(std::size_t, SAFE_CAST(std::float16_t, cor_x) / New_N * Old_N);
        //                         cor_y = SAFE_CAST(std::size_t, SAFE_CAST(std::float16_t, cor_y) / New_M * Old_M);

        //                         return src[cor_x][cor_y];
        //                     });

        // std::ranges::copy(resized_image, flattened_image.begin());

        // return flattened_image;

        std::vector<std::size_t> row_indexes(New_N);
        std::iota(row_indexes.begin(), row_indexes.end(), 0);

        std::for_each(std::execution::par, row_indexes.begin(), row_indexes.end(), 
            [&new_image, &src](std::size_t row_index) -> void {
            for (std::size_t col_index : std::views::iota(0uz, New_M)) {
                const std::size_t mapped_x = row_index * Old_N / New_N;
                const std::size_t mapped_y = col_index * Old_M / New_M;

                new_image[row_index][col_index] = src[mapped_x][mapped_y];
            }
        });

        return new_image;
    }

    template <std::size_t N, std::size_t M>
    inline void print_image(const RGBImage<N, M>& image) {
        auto print_rgb = [](uint8_t r, uint8_t g, uint8_t b) -> void {
            std::print("\033[38;2;{};{};{}m{}\033[0m", r, g, b, "0");
        };

        for (std::size_t i : std::views::iota(0uz, N)) {
            for (std::size_t j : std::views::iota(0uz, M)) {
                print_rgb(image[i][j][0], image[i][j][1], image[i][j][2]);
            }

            std::print("\n");
        }
    }

    [[nodiscard]] constexpr GreyPixel convert_rgb_to_grey(const RGBPixel& pixel) noexcept {
        return 0.299f16 * pixel[0] + 0.587f16 * pixel[1] + 0.114f16 * pixel[2];
    }

    template <std::size_t N, std::size_t M>
    [[nodiscard]] GreyImage<N, M> convert_rgb_to_grey_img(const RGBImage<N, M>& src) noexcept {
        GreyImage<N, M> return_grey_image{};
        for (std::size_t i : std::views::iota(0uz, N)) {
            for (std::size_t j : std::views::iota(0uz, M)) {
                return_grey_image[i][j] = convert_rgb_to_grey(src[i][j]);
            }
        }

        return return_grey_image;
    }
}
