cmake_minimum_required(VERSION 2.8.8)
project (LanternBeacon)

# Files required to build
set (SOURCE_FILES main.cpp krm_detector.cpp)
set (HEADER_FILES krm_detector.hpp)

# Enable pthread library 
set(CMAKE_C_FLAGS -pthread)

include_directories(../KromekDriver/include ../SpectrometerDriver)
add_executable(LanternBeacon ${SOURCE_FILES})

target_link_libraries(LanternBeacon SpectrometerDriver  -lpthread)