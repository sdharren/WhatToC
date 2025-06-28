#include "Engine.h"

Engine::Engine() : board(Board()), move_generator(MoveGenerator())
{

}

Engine::Engine(std::string FEN_string) : board(Board()), move_generator(MoveGenerator())
{
    board.parse_FEN(FEN_string);
}


bool Engine::make_move(Move move)
{
    // copy history
    // auto current_game_state = game_state;
    // current_game_state.next_move = move;
    // history.push_back(current_game_state);
    auto copy_piece_bitboards = board.piece_bitboards;
    auto copy_white_occupancy = board.white_occupancy;
    auto copy_black_occupancy = board.black_occupancy;
    auto copy_occupied_squares = board.occupied_squares;
    auto copy_piece_list = board.piece_list;
    auto copy_game_state = board.game_state;

    // get squares
    int start_square = get_start_square(move);
    int target_square = get_target_square(move);
    int piece = board.piece_list[start_square];
    int side = board.game_state.side_to_move == white ? 0 : 6;

    // flags
    int is_quiet = is_quiet_move(move);
    int is_double_pawn_push = is_double_pawn_push_move(move);
    int is_kingside_castle = is_kingside_castle_move(move);
    int is_queenside_castle = is_queenside_castle_move(move);
    int is_capture = is_capture_move(move);
    int is_ep = is_ep_move(move);
    int is_knight_promo = is_knight_promo_move(move);
    int is_bishop_promo = is_bishop_promo_move(move);
    int is_rook_promo = is_rook_promo_move(move);
    int is_queen_promo = is_queen_promo_move(move);
    int is_knight_capture_promo = is_knight_capture_promo_move(move);
    int is_bishop_capture_promo = is_bishop_capture_promo_move(move);
    int is_rook_capture_promo = is_rook_capture_promo_move(move);
    int is_queen_capture_promo = is_queen_capture_promo_move(move);

    // assume move increments halfmove counter
    board.game_state.halfmove_clock++;

    // reset ep square
    board.set_ep_square(64);

    if (is_quiet)
    {
        board.move_piece(piece, start_square, target_square);
        if (piece == P || piece == p)
        {
            board.game_state.halfmove_clock = 0;
        }
    }

    if (is_double_pawn_push)
    {
        board.move_piece(piece, start_square, target_square);
        board.game_state.halfmove_clock = 0;
        int offset = board.game_state.side_to_move == white ? -1 : 1;
        board.set_ep_square(target_square + (offset * 8));
    }

    if (is_kingside_castle)
    {
        board.move_piece(piece, start_square, target_square);
        switch (target_square)
        {
            case g1:
                board.move_piece(R, h1, f1);
                break;
            case g8:
                board.move_piece(r, h8, f8);
                break;
        }
    }

    if (is_queenside_castle)
    {
        board.move_piece(piece, start_square, target_square);
        switch (target_square)
        {
        case c1:
            board.move_piece(R, a1, d1);
            break;
        case c8:
            board.move_piece(r, a8, d8);
            break;
        }
    }

    if (is_capture)
    {
        // std::cout << "we are in correct branch" << std::endl;
        // std::cout << piece << std::endl;
        int target_piece = board.piece_list[target_square];
        // std::cout << target_piece << std::endl;
        board.remove_piece(target_piece, target_square);
        board.move_piece(piece, start_square, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_ep)
    {
        board.move_piece(piece, start_square, target_square);
        int offset = board.game_state.side_to_move == white ? -8 : 8;
        int target_piece = board.game_state.side_to_move == white ? p : P;
        board.remove_piece(target_piece, target_square + offset);
        board.game_state.halfmove_clock = 0;
    }

    if (is_knight_promo)
    {
        board.remove_piece(piece, start_square);
        board.put_piece(N + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_bishop_promo)
    {
        board.remove_piece(piece, start_square);
        board.put_piece(B + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_rook_promo)
    {
        board.remove_piece(piece, start_square);
        board.put_piece(R + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_queen_promo)
    {
        board.remove_piece(piece, start_square);
        board.put_piece(Q + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_knight_capture_promo)
    {
        int target_piece = board.piece_list[target_square];
        board.remove_piece(piece, start_square);
        board.remove_piece(target_piece, target_square);
        board.put_piece(N + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_bishop_capture_promo)
    {
        int target_piece = board.piece_list[target_square];
        board.remove_piece(piece, start_square);
        board.remove_piece(target_piece, target_square);
        board.put_piece(B + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_rook_capture_promo)
    {
        int target_piece = board.piece_list[target_square];
        board.remove_piece(piece, start_square);
        board.remove_piece(target_piece, target_square);
        board.put_piece(R + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    if (is_queen_capture_promo)
    {
        int target_piece = board.piece_list[target_square];
        board.remove_piece(piece, start_square);
        board.remove_piece(target_piece, target_square);
        board.put_piece(Q + side, target_square);
        board.game_state.halfmove_clock = 0;
    }

    // update castling rights
    board.update_castling_rights(start_square, target_square);

    // increment fullmove counter
    if (board.game_state.side_to_move == black) board.game_state.fullmove_counter++;

    // swap side
    board.swap_side();

    // check if move is legal
    int king = board.game_state.side_to_move == white ? k : K;
    int k_square = get_LS1B(board.piece_bitboards[king]);
    if (move_generator.is_square_attacked_by_colour(k_square, board.game_state.side_to_move, board))
    {
        board.piece_bitboards = copy_piece_bitboards;
        board.white_occupancy = copy_white_occupancy;
        board.black_occupancy = copy_black_occupancy;
        board.occupied_squares = copy_occupied_squares;
        board.piece_list = copy_piece_list;
        board.game_state = copy_game_state;

        return false;
    }
    return true;
}