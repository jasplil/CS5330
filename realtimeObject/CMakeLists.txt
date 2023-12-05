cmake_minimum_required(VERSION 3.9)

project(realtimeObject)

set(CMAKE_CXX_STANDARD 11)

add_executable(realtimeObject ./src/main.cpp ./src/thresholding.cpp)

find_package(OpenCV REQUIRED)

target_link_libraries(realtimeObject ${OpenCV_LIBRARIES})