cmake_minimum_required(VERSION 3.25)

project (ML-DM-2026.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(TBB REQUIRED COMPONENTS tbb)

add_executable(test_resize test_resize.cpp)
add_executable(test_hog test_hog.cpp)

foreach(target IN ITEMS test_resize test_hog)
    target_include_directories(${target} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )

    target_link_libraries(${target} PRIVATE
        TBB::tbb
    )
    
endforeach()