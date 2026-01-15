cmake_minimum_required(VERSION 3.13)
project(opencv_samples)
set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()

add_executable(camera camera.cpp)

option(ENABLE_HARD "Enable GPU rendering" OFF)

if (ENABLE_HARD)
    target_compile_definitions(camera PRIVATE ENABLE_HARD)
endif ()

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED PATHS /usr/local)

foreach (target camera)
    target_include_directories(${target} PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${target} PUBLIC ${OpenCV_LIBRARIES})
endforeach ()