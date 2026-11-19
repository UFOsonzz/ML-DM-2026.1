#include "src/HOG.tpp"
#include "support/test_generate_image.tpp"
#include <iostream>
#include <print>

int32_t main() {
    std::size_t test_num;
    std::cin >> test_num;
    for (std::size_t test_num : std::views::iota(1uz, test_num + 1)) {
        std::println(stderr, "------------test number {}:-------------", test_num);
        image::RGBImage<128, 128> src_image = image_generator::generate<128, 128>();
        std::println(stderr, "--------source RGB image--------");
        image::print_image(src_image);
        std::println(stderr, "--------converted hog vector-------");
        auto hog_vec = hog::run_hog<128, 128, 9>(src_image);
        std::println(stderr, "{}", hog_vec);
        std::println(stderr, "-----------END TEST {}----------", test_num);
    }
}