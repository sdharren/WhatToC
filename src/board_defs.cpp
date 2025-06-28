#include "board_defs.h"

#include <iostream>

// return if a bit is set
Bitboard get_bit(Bitboard bitboard, int square)
{
    return bitboard & (1ULL << square);
}

// set a bit to 1 at a position
void set_bit(Bitboard& bitboard, int square)
{
    bitboard |= (1ULL << square);
}

// set a bit to 0 at a position
void reset_bit(Bitboard& bitboard, int square)
{
    bitboard &= (~(1ULL << square));
}

// count the number of 1s in a bitboard
int count_bits(Bitboard bitboard)
{
    int count = 0;
    while (bitboard)
    {
        count++;
        bitboard = bitboard & (bitboard - 1);
    }

    return count;
}

// get the index of the LS1B
int get_LS1B(Bitboard bitboard)
{
    return count_bits((bitboard & -bitboard) - 1);
}

// Pretty print a bitboard
void print(Bitboard bitboard)
{
    std::cout << "     Bitboard: " << bitboard << std::endl;
    for (int rank = 8; rank > 0; rank--)
    {
        std::cout << rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = (rank - 1) * 8 + file;
            char c = get_bit(bitboard, square) ? 'X' : '.';
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "    a b c d e f g h" << std::endl;
}