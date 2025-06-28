#ifndef FIND_MAGICS_H
#define FIND_MAGICS_H
#include "board_defs.h"

unsigned long long generate_magic_number();
Bitboard find_magic_number(int square, int piece);
void find_all_magic_numbers();

#endif //FIND_MAGICS_H
