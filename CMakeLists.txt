cmake_minimum_required(VERSION 3.17)
project(Risk)

set(CMAKE_CXX_STANDARD 14)

add_executable(Risk MapLoader/MapLoaderDriver.cpp MapLoader/MapLoader.h MapLoader/MapLoader.cpp Map/Map.h Map/Map.cpp Map/MapDriver.cpp Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp)
