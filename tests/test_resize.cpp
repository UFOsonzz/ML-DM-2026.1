#include "support/test_random.h"
#include "include/image.h"
#include "support/test_generate_image.tpp"
#include <print>
#include <iostream>

int32_t main() {
    std::size_t test_num;
    std::cin >> test_num;
    for (std::size_t test_id : std::views::iota(1uz, test_num + 1)) {
        image::RGBImage<64, 64> random_image = image_generator::generate<64, 64>();
        image::RGBImage<128, 128> resized_image = image::resize<128, 128, 64, 64>(random_image);
        std::print(stderr, "------ test number {} ------\n", test_id);
        std::print("before:\n");
        image::print_image<64, 64>(random_image);
        std::print("after:\n");
        image::print_image<128, 128>(resized_image);
    }
}