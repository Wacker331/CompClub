cmake_minimum_required(VERSION 3.31)
project(CompClub)

include_directories(include)

set(SOURCES src/CompClub.cpp src/Club.cpp)

add_executable(CompClub ${SOURCES})