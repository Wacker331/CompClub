cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 11)
project(CompClub)

include_directories(include)

set(SOURCES src/CompClub.cpp src/Club.cpp)

add_executable(CompClub ${SOURCES})