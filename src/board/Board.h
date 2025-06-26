#ifndef BOARD_H
#define BOARD_H
#include <vector>

#include "ZobristRandoms.h"
#include "../helpers/board_defs.h"
#include "../movegen/move.h"

inline std::vector<int> CASTLING_RIGHTS = {
    11, 15, 15, 15, 3, 15, 15, 7,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    14, 15, 15, 15, 12, 15, 15, 13
};

struct GameState
{
    int side_to_move;
    int castling_rights;
    int ep_square;
    int halfmove_clock;
    int fullmove_counter;
    Bitboard zobrist_key;
    Move next_move;
};

// Board class
class Board
{
public:
    std::vector<Bitboard> piece_bitboards;
    Bitboard white_occupancy;
    Bitboard black_occupancy;
    Bitboard occupied_squares;
    std::vector<int> piece_list;

    GameState game_state;
    std::vector<GameState> history;

    ZobristRandoms zobrist_randoms;

    Board();
    void parse_FEN(std::string FEN_string);

    void put_piece(int piece, int square);
    void remove_piece(int piece, int square);
    void move_piece(int piece, int start_square, int target_square);
    void set_ep_square(int square);
    void swap_side();
    void update_castling_rights(int start_square, int target_square);

    bool make_move(Move move);
};

// print bitboard
void print(Bitboard bitboard);

#endif //BOARD_H
