#ifndef MOVE_H
#define MOVE_H
#include <cstdint>

using Move = uint16_t;

Move create_move(int start_square, int target_square, int move_flags);

int get_start_square(Move move);
int get_target_square(Move move);

Move shift_to_flags(Move move);
int is_quiet_move(Move move);
int is_double_pawn_push_move(Move move);
int is_kingside_castle_move(Move move);
int is_queenside_castle_move(Move move);
int is_capture_move(Move move);
int is_ep_move(Move move);
int is_knight_promo_move(Move move);
int is_bishop_promo_move(Move move);
int is_rook_promo_move(Move move);
int is_queen_promo_move(Move move);
int is_knight_capture_promo_move(Move move);
int is_bishop_capture_promo_move(Move move);
int is_rook_capture_promo_move(Move move);
int is_queen_capture_promo_move(Move move);

#endif //MOVE_H
