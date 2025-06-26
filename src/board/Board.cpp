#include "Board.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "../helpers/bit_helpers.h"
#include "../movegen/move_gen.h"

Board::Board() : piece_bitboards(12), white_occupancy(0), black_occupancy(0), occupied_squares(0), piece_list(64),
                 game_state(GameState()), zobrist_randoms(ZobristRandoms())
{

}

// function to pretty print the bitboard, for debugging
void print(Bitboard bitboard)
{
    std::cout << "     Bitboard: " << bitboard << std::endl;
    for (int rank = 8; rank > 0; rank--)
    {
        std::cout << rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = (rank - 1) * 8 + file;
            char c = get_bit(bitboard, square) ? 'X' : '.';
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "    a b c d e f g h" << std::endl;
}

void Board::parse_FEN(std::string FEN_string)
{
    std::fill(piece_list.begin(), piece_list.end(), 12);
    // split string by space
    std::string s;
    std::stringstream ss(FEN_string);
    std::vector<std::string> fields;

    while (getline(ss, s, ' '))
    {
        fields.push_back(s);
    }

    // parse board state
    std::vector<std::string> ranks;
    ss.clear();
    ss.str(fields[0]);

    while (getline(ss, s, '/'))
    {
        ranks.push_back(s);
    }
    int rank_index = 7;
    for (auto rank: ranks)
    {
        int file_index = 0;
        for (auto c: rank)
        {
            if (std::isdigit(c))
            {
                int num = c - '0';
                file_index = file_index + num;
            }
            else
            {
                int piece = char_to_piece[c];
                int square = (rank_index * 8) + file_index;

                // set piece in piece_list
                piece_list[square] = piece;

                // set piece on bitboard
                set_bit(piece_bitboards[piece], square);

                // update zobrist key
                game_state.zobrist_key ^= zobrist_randoms.piece_randoms[piece][square];
                file_index++;
            }
        }
        rank_index--;
    }
    // set white occupancy
    for (int i = K; i <= P; i++)
    {
        white_occupancy |= piece_bitboards[i];
    }

    // set black occupancy
    for (int i = k; i <= p; i++)
    {
        black_occupancy |= piece_bitboards[i];
    }

    // set occupied squares
    occupied_squares |= white_occupancy;
    occupied_squares |= black_occupancy;

    // parse side to move
    game_state.side_to_move = fields[1] == "w" ? 0 : 1;
    game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];

    // parse castling rights
    if (fields[2] == "-")
    {
        game_state.castling_rights = 0;
    }
    else
    {
        for (auto c: fields[2])
        {
            if (c == 'K') game_state.castling_rights |= (1 << 3);
            if (c == 'Q') game_state.castling_rights |= (1 << 2);
            if (c == 'k') game_state.castling_rights |= (1 << 1);
            if (c == 'q') game_state.castling_rights |= 1;
        }
    }
    game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];

    // parse en passant square
    game_state.ep_square = char_to_sq[fields[3]];
    game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];

    // parse halfmove clock
    game_state.halfmove_clock = std::stoi(fields[4]);

    // parse fullmove counter
    game_state.fullmove_counter = std::stoi(fields[5]);

}

void Board::put_piece(int piece, int square)
{
    // set piece on bitboard
    set_bit(piece_bitboards[piece], square);

    // update zobrist key
    game_state.zobrist_key = zobrist_randoms.piece_randoms[piece][square];

    // update piece list
    piece_list[square] = piece;

    // update occupancy
    if (piece <= 5)
    {
        set_bit(white_occupancy, square);
    }
    else
    {
        set_bit(black_occupancy, square);
    }
    set_bit(occupied_squares, square);

    // update evaluation TODO
}

void Board::remove_piece(int piece, int square)
{
    // remove piece on bitboard
    reset_bit(piece_bitboards[piece], square);

    // update zobrist key
    game_state.zobrist_key = zobrist_randoms.piece_randoms[piece][square];

    // update piece list
    piece_list[square] = 12;

    // update occupancy
    if (piece <= 5)
    {
        reset_bit(white_occupancy, square);
    }
    else
    {
        reset_bit(black_occupancy, square);
    }
    reset_bit(occupied_squares, square);

    // update evaluation TODO
}

void Board::move_piece(int piece, int start_square, int target_square)
{
    // remove piece
    remove_piece(piece, start_square);

    // put piece
    put_piece(piece, target_square);
}

void Board::set_ep_square(int square)
{
    // remove current ep from zobrist
    game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];

    // set eq square
    game_state.ep_square = square;

    // add new ep to zobrist
    game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];
}

void Board::swap_side()
{
    // remove from zobrist
    game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];

    // change side
    game_state.side_to_move ^= 1;

    // update zobrist
    game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];
}

void Board::update_castling_rights(int start_square, int target_square)
{
    // remove from zobrist
    game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];

    // update castling rights
    game_state.castling_rights &= CASTLING_RIGHTS[start_square];
    game_state.castling_rights &= CASTLING_RIGHTS[target_square];

    // update zobrist
    game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];
}

bool Board::make_move(Move move)
{
    // copy history
    // auto current_game_state = game_state;
    // current_game_state.next_move = move;
    // history.push_back(current_game_state);
    auto copy_piece_bitboards = piece_bitboards;
    auto copy_white_occupancy = white_occupancy;
    auto copy_black_occupancy = black_occupancy;
    auto copy_occupied_squares = occupied_squares;
    auto copy_piece_list = piece_list;
    auto copy_game_state = game_state;

    // get squares
    int start_square = get_start_square(move);
    int target_square = get_target_square(move);
    int piece = piece_list[start_square];
    int side = game_state.side_to_move == white ? 0 : 6;

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
    game_state.halfmove_clock++;

    // reset ep square
    game_state.ep_square = no_sq;

    if (is_quiet)
    {
        move_piece(piece, start_square, target_square);
        if (piece == P || piece == p)
        {
            game_state.halfmove_clock = 0;
        }
    }

    if (is_double_pawn_push)
    {
        move_piece(piece, start_square, target_square);
        game_state.halfmove_clock = 0;
        int offset = game_state.side_to_move == white ? -1 : 1;
        set_ep_square(target_square + (offset * 8));
    }

    if (is_kingside_castle)
    {
        move_piece(piece, start_square, target_square);
        switch (target_square)
        {
            case g1:
                move_piece(R, h1, f1);
                break;
            case g8:
                move_piece(r, h8, f8);
                break;
        }
    }

    if (is_queenside_castle)
    {
        move_piece(piece, start_square, target_square);
        switch (target_square)
        {
        case c1:
            move_piece(R, a1, d1);
            break;
        case c8:
            move_piece(r, a8, d8);
            break;
        }
    }

    if (is_capture)
    {
        int target_piece = piece_list[target_square];
        move_piece(piece, start_square, target_square);
        remove_piece(target_piece, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_ep)
    {
        move_piece(piece, start_square, target_square);
        int target_piece = game_state.side_to_move == white ? p : P;
        remove_piece(target_piece, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_knight_promo)
    {
        remove_piece(piece, start_square);
        put_piece(N + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_bishop_promo)
    {
        remove_piece(piece, start_square);
        put_piece(B + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_rook_promo)
    {
        remove_piece(piece, start_square);
        put_piece(R + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_queen_promo)
    {
        remove_piece(piece, start_square);
        put_piece(Q + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_knight_capture_promo)
    {
        int target_piece = piece_list[target_square];
        remove_piece(piece, start_square);
        remove_piece(target_piece, target_square);
        put_piece(N + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_bishop_capture_promo)
    {
        int target_piece = piece_list[target_square];
        remove_piece(piece, start_square);
        remove_piece(target_piece, target_square);
        put_piece(B + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_rook_capture_promo)
    {
        int target_piece = piece_list[target_square];
        remove_piece(piece, start_square);
        remove_piece(target_piece, target_square);
        put_piece(R + side, target_square);
        game_state.halfmove_clock = 0;
    }

    if (is_queen_capture_promo)
    {
        int target_piece = piece_list[target_square];
        remove_piece(piece, start_square);
        remove_piece(target_piece, target_square);
        put_piece(Q + side, target_square);
        game_state.halfmove_clock = 0;
    }

    // update castling rights
    game_state.castling_rights &= CASTLING_RIGHTS[start_square];
    game_state.castling_rights &= CASTLING_RIGHTS[target_square];

    // increment fullmove counter
    if (game_state.side_to_move == black) game_state.fullmove_counter++;

    // swap side
    swap_side();

    // check if move is legal
    int king = game_state.side_to_move == white ? k : K;
    int k_square = get_LS1B(piece_bitboards[king]);
    auto mg = MoveGenerator();
    if (mg.is_square_attacked_by_colour(k_square, game_state.side_to_move, *this))
    {
        piece_bitboards = copy_piece_bitboards;
        white_occupancy = copy_white_occupancy;
        black_occupancy = copy_black_occupancy;
        occupied_squares = copy_occupied_squares;
        piece_list = copy_piece_list;
        game_state = copy_game_state;

        return false;
    }


    return true;
}
