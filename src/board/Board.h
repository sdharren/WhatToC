#ifndef BOARD_H
#define BOARD_H
#include <vector>

#include "ZobristRandoms.h"
#include "../helpers/board_defs.h"

// Board class
class Board
{
public:
    std::vector<Bitboard> piece_bitboards;
    Bitboard white_occupancy;
    Bitboard black_occupancy;
    Bitboard occupied_squares;
    std::vector<int> piece_list;

    int side_to_move;
    int castling_rights;
    int ep_square;
    int halfmove_clock;
    int fullmove_counter;

    ZobristRandoms zobrist_randoms;
    Bitboard zobrist_key;

    Board();
    void parse_FEN(std::string FEN_string);
};

// print bitboard
void print(Bitboard bitboard);

#endif //BOARD_H
