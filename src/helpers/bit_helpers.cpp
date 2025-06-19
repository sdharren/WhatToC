#include "bit_helpers.h"

Bitboard get_bit(Bitboard bitboard, int square)
{
    return bitboard & (1ULL << square);
}

void set_bit(Bitboard& bitboard, int square)
{
    bitboard |= (1ULL << square);
}

void reset_bit(Bitboard& bitboard, int square)
{
    bitboard &= (~(1ULL << square));
}

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

int get_LS1B(Bitboard bitboard)
{
    return count_bits((bitboard & -bitboard) - 1);
}