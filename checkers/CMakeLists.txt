cmake_minimum_required(VERSION 3.20)
project(checkers)

set(CMAKE_CXX_STANDARD 23)

add_executable(checkers main.cpp checker.h checker.cpp board.h board.cpp player.h game.h game.cpp)
