cmake_minimum_required(VERSION 3.26)
project(encryption)

set(CMAKE_CXX_STANDARD 17)

add_executable(encryption main.cpp
        Encryption.cpp
        Encryption.h)
