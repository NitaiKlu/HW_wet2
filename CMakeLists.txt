cmake_minimum_required(VERSION 3.0.0)
project(wet2 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(wet2 
Game.cpp
Group.cpp
Level.cpp
LevelTree.cpp
Player.cpp
library2.cpp
main2.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
