cmake_minimum_required(VERSION 3.20)
project(imageSteganography)

set(CMAKE_CXX_STANDARD 17)

add_executable(imageSteganography main.cpp bmp.hpp png.hpp app.hpp)
