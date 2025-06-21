cmake_minimum_required(VERSION 3.31)
project(WhatToC)

set(CMAKE_CXX_STANDARD 20)

add_executable(WhatToC src/main.cpp
        src/board/Board.cpp
        src/board/Board.h
        src/helpers/bit_helpers.cpp
        src/helpers/bit_helpers.h
        src/helpers/prng.cpp
        src/helpers/prng.h
        src/movegen/move_gen.cpp
        src/movegen/move_gen.h
        src/movegen/find_magics.cpp
        src/movegen/find_magics.h
        src/helpers/board_defs.h
        src/board/ZobristRandoms.cpp
        src/board/ZobristRandoms.h
)
