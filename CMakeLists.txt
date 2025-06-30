cmake_minimum_required(VERSION 3.16)
project(opencv_samples)
set(CMAKE_CXX_STANDARD 11)
add_executable(camera camera.cpp)
add_executable(video_capture_from_local_file video_capture_from_local_file.cpp)

set(OPENCV_CPP_SAMPLES_REQUIRED_DEPS
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
        opencv_videoio
        opencv_highgui)

include_directories(/usr/local/include/opencv4)
link_directories(/usr/local/lib)

foreach (target camera video_capture_from_local_file)
    target_link_libraries(${target} PUBLIC ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})
endforeach ()

