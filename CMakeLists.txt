cmake_minimum_required(VERSION 3.0.0)
project(DataSructuresHW2 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MAKE_PROGRAM C:/Program Files/CMake/bin/cmake.exe FORCE )

include(CTest)
enable_testing()
set(CMAKE_BUILD_TYPE Debug)
set(FLAGS_DEBUG "-std=c++11 --pedantic-errors -Wall -Werror")
set(CMAKE_CPP_FLAGS ${FLAGS_DEBUG})
set(CPP_FILES 
./HashTest.cpp
)
add_executable(HW_WET2 ${CPP_FILES})

