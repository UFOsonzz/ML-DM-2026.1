#include "test_random.tpp"
#include "include/image.h"

namespace image_generator {
    template <std::size_t N, std::size_t M>
    [[nodiscard]] image::RGBImage<N, M> generate() noexcept {
        image::RGBImage<N, M> new_image;
        std::vector<std::size_t> row_indexes(N);
        std::iota(row_indexes.begin(), row_indexes.end(), 0);
        std::for_each(std::execution::par, row_indexes.begin(), row_indexes.end(), 
        [&new_image](std::size_t row_index) {
            for (std::size_t col_index : std::views::iota(0uz, M)) {
                new_image[row_index][col_index] = {
                    random_generator::get_uniform_random<uint8_t>(0u, 255u),
                    random_generator::get_uniform_random<uint8_t>(0u, 255u),
                    random_generator::get_uniform_random<uint8_t>(0u, 255u)
                };
            }
        });

        return new_image;
    }
}