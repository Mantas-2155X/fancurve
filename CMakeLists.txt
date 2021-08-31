cmake_minimum_required(VERSION 3.18)
project(fancurve)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

add_executable(fancurve main.cpp main.h)

install(TARGETS fancurve DESTINATION bin)