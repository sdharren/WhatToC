#ifndef FIND_MAGICS_H
#define FIND_MAGICS_H
#include "../helpers/board_defs.h"


Bitboard generate_blocker_board(int index, int num_of_blockers, Bitboard attack_mask);
Bitboard generate_rook_attack_map_with_blockers(int square, Bitboard blocker_board);
Bitboard generate_bishop_attack_map_with_blockers(int square, Bitboard blocker_board);

unsigned long long generate_magic_number();
Bitboard find_magic_number(int square, int piece);
void find_all_magic_numbers();

#endif //FIND_MAGICS_H
