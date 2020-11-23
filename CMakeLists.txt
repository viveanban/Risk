cmake_minimum_required(VERSION 3.17)
project(Risk)

set(CMAKE_CXX_STANDARD 14)

add_executable(Risk MapLoader/MapLoader.h MapLoader/MapLoader.cpp MapLoader/ConquestFileReader.cpp MapLoader/ConquestFileReader.h  Map/Map.h Map/Map.cpp Orders/Orders.cpp Orders/Orders.h Cards/Cards.cpp Cards/Cards.h Player/Player.cpp Player/Player.h GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp GameObservers/GameObservers.cpp GameObservers/GameObservers.h)
