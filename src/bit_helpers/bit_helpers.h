#ifndef BIT_HELPERS_H
#define BIT_HELPERS_H
#include "../board/Board.h"

Bitboard get_bit(Bitboard bitboard, int square);

void set_bit(Bitboard& bitboard, int square);

void reset_bit(Bitboard& bitboard, int square);

#endif //BIT_HELPERS_H
