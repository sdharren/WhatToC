#ifndef BOARD_H
#define BOARD_H

#include "board_defs.h"
#include "Move.h"
#include "ZobristRandoms.h"

#include <array>
#include <vector>

// Board constants
inline constexpr std::array<int, 64> CASTLING_RIGHTS = {
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

    bool operator==(const GameState& gs2);
};

class Board {
public:
    std::vector<Bitboard> piece_bitboards;
    Bitboard white_occupancy;
    Bitboard black_occupancy;
    Bitboard occupied_squares;
    std::vector<int> piece_list;
    ZobristRandoms zobrist_randoms;

    GameState game_state;

    Board();

    // utility functions
    void parse_FEN(std::string FEN_string);
    void put_piece(int piece, int square, bool is_unmake);
    void remove_piece(int piece, int square, bool is_unmake);
    void move_piece(int piece, int start_square, int target_square, bool is_unmake);
    void set_ep_square(int square);
    void swap_side();
    void update_castling_rights(int start_square, int target_square);

    // debugging functions
    void print_piece_list();

    bool operator==(const Board& b2);
};

#endif //BOARD_H
