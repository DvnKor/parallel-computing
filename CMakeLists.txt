cmake_minimum_required(VERSION 3.22)
project(parallel_computing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

file(GLOB APP_SOURCES *.cpp */*.cpp)
foreach (testsourcefile ${APP_SOURCES})
    get_filename_component(testname ${testsourcefile} NAME_WE)
    message("${testname}")
    add_executable(${testname} ${testsourcefile})
endforeach (testsourcefile ${APP_SOURCES})

