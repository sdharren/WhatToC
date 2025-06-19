cmake_minimum_required(VERSION 3.31)
project(WhatToC)

set(CMAKE_CXX_STANDARD 20)

add_executable(WhatToC src/main.cpp
        src/board/Board.cpp
        src/board/Board.h
        src/bit_helpers/bit_helpers.cpp
        src/bit_helpers/bit_helpers.h
)
