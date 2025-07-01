cmake_minimum_required(VERSION 3.13)
project(opencv_samples)
set(CMAKE_CXX_STANDARD 11)
add_executable(camera camera.cpp)
add_executable(video_capture_from_local_file video_capture_from_local_file.cpp)

find_package(OpenCV REQUIRED PATHS /usr/local)

foreach (target camera video_capture_from_local_file)
    target_include_directories(${target} PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${target} PUBLIC ${OpenCV_LIBRARIES})
endforeach ()

