#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <array>

#include "../board/Board.h"

class MoveGenerator
{
public:
    std::array<std::array<Bitboard, 64>, 2> PAWN_ATTACK_TABLE;
    std::array<Bitboard, 64> KNIGHT_ATTACK_TABLE;
    std::array<Bitboard, 64> KING_ATTACK_TABLE;

    MoveGenerator();
    void initialise_leaper_piece_attack_tables();
    Bitboard generate_pawn_attack_from_square(int square, int colour);
    Bitboard generate_knight_attack_from_square(int square);
    Bitboard generate_king_attack_from_square(int square);
    Bitboard generate_rook_attack_mask_from_square(int square);
    Bitboard generate_bishop_attack_mask_from_square(int square);
};

#endif //MOVE_GEN_H
