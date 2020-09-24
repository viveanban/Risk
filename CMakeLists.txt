cmake_minimum_required(VERSION 3.17)
project(Risk)

set(CMAKE_CXX_STANDARD 14)

add_executable(Risk MapLoaderDriver.cpp MapLoader.h MapLoader.cpp)