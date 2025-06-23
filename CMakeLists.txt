cmake_minimum_required(VERSION 3.31)
project(opencv_samples)
set(CMAKE_CXX_STANDARD 20)
add_executable(camera camera.cpp)

set(OPENCV_CPP_SAMPLES_REQUIRED_DEPS
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
        opencv_videoio
        opencv_highgui)

target_include_directories(camera PUBLIC /usr/local/include/opencv4)
target_link_libraries(camera PUBLIC ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})
target_link_directories(camera PUBLIC /usr/local/lib)