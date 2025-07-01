#ifndef MOVE_H
#define MOVE_H
#include <cstdint>

enum MoveType
{
    quiet, double_pawn_push, kingside_castle, queenside_castle, capture, ep, knight_promo, bishop_promo, rook_promo,
    queen_promo, knight_capture_promo, bishop_capture_promo, rook_capture_promo, queen_capture_promo
};

// Standardised Move type
using Move = uint16_t;

// Move functions
Move create_move(int start_square, int target_square, int move_flags);
constexpr Move shift_to_flags(Move move) {return move >> 12;}

constexpr int get_start_square(Move move) {return move & 0b111111;}
constexpr int get_target_square(Move move) {return (move >> 6) & 0b111111;}
constexpr int is_quiet_move(Move move) {return shift_to_flags(move) == quiet;};
constexpr int is_double_pawn_push_move(Move move) {return shift_to_flags(move) == double_pawn_push;};
constexpr int is_kingside_castle_move(Move move) {return shift_to_flags(move) == kingside_castle;};
constexpr int is_queenside_castle_move(Move move) {return shift_to_flags(move) == queenside_castle;};
constexpr int is_capture_move(Move move) {return shift_to_flags(move) == capture;};
constexpr int is_ep_move(Move move) {return shift_to_flags(move) == ep;};
constexpr int is_knight_promo_move(Move move) {return shift_to_flags(move) == knight_promo;};
constexpr int is_bishop_promo_move(Move move) {return shift_to_flags(move) == bishop_promo;};
constexpr int is_rook_promo_move(Move move) {return shift_to_flags(move) == rook_promo;};
constexpr int is_queen_promo_move(Move move) {return shift_to_flags(move) == queen_promo;};
constexpr int is_knight_capture_promo_move(Move move) {return shift_to_flags(move) == knight_capture_promo;};
constexpr int is_bishop_capture_promo_move(Move move) {return shift_to_flags(move) == bishop_capture_promo;};
constexpr int is_rook_capture_promo_move(Move move) {return shift_to_flags(move) == rook_capture_promo;};
constexpr int is_queen_capture_promo_move(Move move) {return shift_to_flags(move) == queen_capture_promo;};
void print_move(Move move);

#endif //MOVE_H
