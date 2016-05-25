cmake_minimum_required(VERSION 2.8)
project(burgers)
find_package(OpenCV REQUIRED)
add_library(burger burger.cpp)
target_link_libraries(burger ${OpenCV_LIBS})

add_executable(burger_test burger_test.cpp)
target_link_libraries(burger_test burger)
