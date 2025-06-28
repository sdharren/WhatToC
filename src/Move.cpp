#include "board_defs.h"
#include "Move.h"
#include <iostream>

// Create a move
Move create_move(int start_square, int target_square, int move_flags)
{
    Move move =  0;
    move |= start_square;
    move |= target_square << 6;
    move |= move_flags << 12;

    return move;
}

int get_start_square(Move move)
{
    return move & 0b111111;
}

int get_target_square(Move move)
{
    return (move >> 6) & 0b111111;
}

Move shift_to_flags(Move move)
{
    return move >> 12;
}

int is_quiet_move(Move move)
{
    return shift_to_flags(move) == 0;
}

int is_double_pawn_push_move(Move move)
{
    return shift_to_flags(move) == 1;
}

int is_kingside_castle_move(Move move)
{
    return shift_to_flags(move) == 2;
}

int is_queenside_castle_move(Move move)
{
    return shift_to_flags(move) == 3;
}

int is_capture_move(Move move)
{
    return shift_to_flags(move) == 4;
}

int is_ep_move(Move move)
{
    return shift_to_flags(move) == 5;
}

int is_knight_promo_move(Move move)
{
    return shift_to_flags(move) == 8;
}

int is_bishop_promo_move(Move move)
{
    return shift_to_flags(move) == 9;
}

int is_rook_promo_move(Move move)
{
    return shift_to_flags(move) == 10;
}

int is_queen_promo_move(Move move)
{
    return shift_to_flags(move) == 11;
}

int is_knight_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 12;
}

int is_bishop_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 13;
}

int is_rook_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 14;
}

int is_queen_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 15;
}

// Pretty print a move, for debugging purposes
void print_move(Move move)
{
    auto ss = get_start_square(move);
    auto ts = get_target_square(move);
    auto x = shift_to_flags(move);
    std::cout << sq_to_char.at(ss) << "" << sq_to_char.at(ts) << "-" << x << ": ";
}