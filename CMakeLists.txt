cmake_minimum_required(VERSION 3.13)
project(opencv_samples)
set(CMAKE_CXX_STANDARD 11)

add_executable(camera camera.cpp)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED PATHS /usr/local)

foreach (target camera)
    target_include_directories(${target} PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${target} PUBLIC ${OpenCV_LIBRARIES})
endforeach ()
