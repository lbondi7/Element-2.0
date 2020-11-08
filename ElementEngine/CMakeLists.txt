cmake_minimum_required(VERSION 3.17)
project(ElementEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(enginelib)
add_subdirectory(app)