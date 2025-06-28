cmake_minimum_required(VERSION 3.31)
project(WhatToC)

set(CMAKE_CXX_STANDARD 20)

add_executable(WhatToC src/main.cpp
        src/board_defs.h
        src/board_defs.cpp
        src/Move.cpp
        src/Move.h
        src/PRNG.cpp
        src/PRNG.h
        src/ZobristRandoms.cpp
        src/ZobristRandoms.h
        src/MoveGenerator.cpp
        src/MoveGenerator.h
        src/Board.cpp
        src/Board.h
        src/Engine.cpp
        src/Engine.h
        src/PERFT.cpp
        src/PERFT.h
        src/main.cpp
        src/perft_tests.cpp
        src/find_magics.h
        src/find_magics.cpp
)
