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