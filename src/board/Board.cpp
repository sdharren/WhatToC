#include "Board.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "../helpers/bit_helpers.h"

Board::Board() : piece_bitboards(12), white_occupancy(0), black_occupancy(0), occupied_squares(0), piece_list(64),
                 game_state(GameState()), zobrist_randoms(ZobristRandoms()), PAWN_ATTACK_TABLE(2, std::vector<Bitboard>(64)), KNIGHT_ATTACK_TABLE(64),
                 KING_ATTACK_TABLE(64), ROOK_ATTACK_TABLE(64, std::vector<Bitboard>(4096)),
                 BISHOP_ATTACK_TABLE(64, std::vector<Bitboard>(512))
{
    initialise_leaper_piece_attack_tables();
    initialise_rook_attack_table();
    initialise_bishop_attack_table();
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
//    auto mg = MoveGenerator();
    if (is_square_attacked_by_colour(k_square, game_state.side_to_move, *this))
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

Bitboard Board::generate_king_attack_from_square(int square)
{
    Bitboard attack_map = 0;
    Bitboard bitboard = 0;
    set_bit(bitboard, square);

    // north
    if (bitboard << 8) attack_map |= bitboard << 8;
    // north east
    if (bitboard << 9 & ~A_FILE_MASK) attack_map |= bitboard << 9;
    // east
    if (bitboard << 1 & ~A_FILE_MASK) attack_map |= bitboard << 1;
    // south east
    if (bitboard >> 7 & ~A_FILE_MASK) attack_map |= bitboard >> 7;
    // south
    if (bitboard >> 8) attack_map |= bitboard >> 8;
    // south west
    if (bitboard >> 9 & ~H_FILE_MASK) attack_map |= bitboard >> 9;
    // west
    if (bitboard >> 1 & ~H_FILE_MASK) attack_map |= bitboard >> 1;
    // north west
    if (bitboard << 7 & ~H_FILE_MASK) attack_map |= bitboard << 7;

    return attack_map;
}

Bitboard Board::generate_knight_attack_from_square(int square)
{
    Bitboard attack_map = 0;
    Bitboard bitboard = 0;
    set_bit(bitboard, square);

    // NNE
    if (bitboard << 17 & ~A_FILE_MASK) attack_map |= bitboard << 17;
    // NEE
    if (bitboard << 10 & ~AB_FILE_MASK) attack_map |= bitboard << 10;
    // SEE
    if (bitboard >> 6 & ~AB_FILE_MASK) attack_map |= bitboard >> 6;
    // SSE
    if (bitboard >> 15 & ~A_FILE_MASK) attack_map |= bitboard >> 15;
    // SSW
    if (bitboard >> 17 & ~H_FILE_MASK) attack_map |= bitboard >> 17;
    // SWW
    if (bitboard >> 10 & ~GH_FILE_MASK) attack_map |= bitboard >> 10;
    // NWW
    if (bitboard << 6 & ~GH_FILE_MASK) attack_map |= bitboard << 6;
    // NNW
    if (bitboard << 15 & ~H_FILE_MASK) attack_map |= bitboard << 15;

    return attack_map;
}

Bitboard Board::generate_pawn_attack_from_square(int square, int colour)
{
    Bitboard attack_map = 0;
    Bitboard bitboard = 0;
    set_bit(bitboard, square);

    if (colour == white)
    {
        if (bitboard << 7 & ~H_FILE_MASK) attack_map |= bitboard << 7;
        if (bitboard << 9 & ~A_FILE_MASK) attack_map |= bitboard << 9;
    }
    else
    {
        if (bitboard >> 7 & ~A_FILE_MASK) attack_map |= bitboard >> 7;
        if (bitboard >> 9 & ~H_FILE_MASK) attack_map |= bitboard >> 9;
    }

    return attack_map;
}

void Board::initialise_leaper_piece_attack_tables()
{
    for (int square = 0; square < 64; square++)
    {
        // generate king attack
        KING_ATTACK_TABLE[square] = generate_king_attack_from_square(square);

        // generate knight attack
        KNIGHT_ATTACK_TABLE[square] = generate_knight_attack_from_square(square);

        // generate pawn attacks
        PAWN_ATTACK_TABLE[white][square] = generate_pawn_attack_from_square(square, white);
        PAWN_ATTACK_TABLE[black][square] = generate_pawn_attack_from_square(square, black);
    }

}

Bitboard Board::generate_rook_attack_mask_from_square(int square)
{
    Bitboard attack_mask = 0;
    int rank =  square / 8;
    int file = square % 8;
    int newSquare;

    // east
    for (int newFile = file + 1; newFile < 7; newFile++)
    {
        newSquare = rank * 8 + newFile;
        set_bit(attack_mask, newSquare);
    }

    // west
    for (int newFile = file - 1; newFile > 0; newFile--)
    {
        newSquare = rank * 8 + newFile;
        set_bit(attack_mask, newSquare);
    }

    // north
    for (int newRank = rank + 1; newRank < 7; newRank++)
    {
        newSquare = newRank * 8 + file;
        set_bit(attack_mask, newSquare);
    }

    // south
    for (int newRank = rank - 1; newRank > 0; newRank--)
    {
        newSquare = newRank * 8 + file;
        set_bit(attack_mask, newSquare);
    }

    return attack_mask;
}

Bitboard Board::generate_bishop_attack_mask_from_square(int square)
{
    Bitboard attack_mask = 0;
    int rank =  square / 8;
    int file = square % 8;
    int newSquare;

    // NE
    for (int newFile = file + 1, newRank = rank + 1; newFile < 7 && newRank < 7; newFile++, newRank++)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_mask, newSquare);
    }

    // SE
    for (int newFile = file + 1, newRank = rank - 1; newFile < 7 && newRank > 0; newFile++, newRank--)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_mask, newSquare);
    }

    // SW
    for (int newFile = file - 1, newRank = rank - 1; newFile > 0 && newRank > 0; newFile--, newRank--)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_mask, newSquare);
    }

    // NW
    for (int newFile = file - 1, newRank = rank + 1; newFile > 0 && newRank < 7; newFile--, newRank++)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_mask, newSquare);
    }

    return attack_mask;
}

void Board::initialise_rook_attack_table()
{
    for (int square = 0; square < 64; square++)
    {
        auto mask = ROOK_ATTACK_MASKS[square];
        auto shift_bits = ROOK_SHIFT_BITS[square];
        int num_of_blocker_boards = 1 << shift_bits;

        // generate blocker board and populate table
        for (int i = 0; i < num_of_blocker_boards; i++)
        {
            Bitboard blocker_board = generate_blocker_board(i, shift_bits, mask);
            int magic_index = (blocker_board * ROOK_MAGICS[square]) >> (64 - shift_bits);
            ROOK_ATTACK_TABLE[square][magic_index] = generate_rook_attack_map_with_blockers(square, blocker_board);
        }
    }
}

void Board::initialise_bishop_attack_table()
{
    for (int square = 0; square < 64; square++)
    {
        auto mask = BISHOP_ATTACK_MASKS[square];
        auto shift_bits = BISHOP_SHIFT_BITS[square];
        int num_of_blocker_boards = 1 << shift_bits;

        // generate blocker board and populate table
        for (int i = 0; i < num_of_blocker_boards; i++)
        {
            Bitboard blocker_board = generate_blocker_board(i, shift_bits, mask);
            int magic_index = (blocker_board * BISHOP_MAGICS[square]) >> (64 - shift_bits);
            BISHOP_ATTACK_TABLE[square][magic_index] = generate_bishop_attack_map_with_blockers(square, blocker_board);
        }
    }
}

Bitboard Board::generate_rook_attack_from_square(int square, Bitboard occupancy)
{
    auto mask = ROOK_ATTACK_MASKS[square];
    mask &= occupancy;
    int magic_index = (mask * ROOK_MAGICS[square]) >> (64 - ROOK_SHIFT_BITS[square]);
    return ROOK_ATTACK_TABLE[square][magic_index];
}

Bitboard Board::generate_bishop_attack_from_square(int square, Bitboard occupancy)
{
    auto mask = BISHOP_ATTACK_MASKS[square];
    mask &= occupancy;
    int magic_index = (mask * BISHOP_MAGICS[square]) >> (64 - BISHOP_SHIFT_BITS[square]);
    return BISHOP_ATTACK_TABLE[square][magic_index];
}

Bitboard Board::generate_queen_attack_from_square(int square, Bitboard occupancy)
{
    auto mask = ROOK_ATTACK_MASKS[square];
    mask &= occupancy;
    int magic_index = (mask * ROOK_MAGICS[square]) >> (64 - ROOK_SHIFT_BITS[square]);
    auto queen_attack = ROOK_ATTACK_TABLE[square][magic_index];

    mask = BISHOP_ATTACK_MASKS[square];
    mask &= occupancy;
    magic_index = (mask * BISHOP_MAGICS[square]) >> (64 - BISHOP_SHIFT_BITS[square]);
    return queen_attack | BISHOP_ATTACK_TABLE[square][magic_index];
}

void Board::generate_piece_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, Board &board,
                                                     int piece)
{
    int side = board.game_state.side_to_move == white ? 0 : 6;
    Bitboard& player_bitboard = board.game_state.side_to_move == white ? board.white_occupancy : board.black_occupancy;
    Bitboard& opponent_bitboard = board.game_state.side_to_move == white ? board.black_occupancy : board.white_occupancy;

    Bitboard bitboard = board.piece_bitboards[piece + side];
    while (bitboard)
    {
        // get start square
        int start_square = get_LS1B(bitboard);
        Bitboard attack_squares;
        switch (piece)
        {
            case K:
                attack_squares = KING_ATTACK_TABLE[start_square];
                break;
            case Q:
                attack_squares = generate_queen_attack_from_square(start_square, board.occupied_squares);
                break;
            case R:
                attack_squares = generate_rook_attack_from_square(start_square, board.occupied_squares);
                break;
            case B:
                attack_squares = generate_bishop_attack_from_square(start_square, board.occupied_squares);
                break;
            case N:
                attack_squares = KNIGHT_ATTACK_TABLE[start_square];
                break;
        }
        attack_squares &= ~player_bitboard;
        while (attack_squares)
        {
            // get target square
            int target_square = get_LS1B(attack_squares);

            // capture
            if (get_bit(opponent_bitboard, target_square))
            {
                move_list[move_count] = create_move(start_square, target_square, 0b0100);
            }
                // quiet
            else
            {
                move_list[move_count] = create_move(start_square, target_square, 0b0);
            }
            move_count++;

            attack_squares &= attack_squares - 1;
        }

        // pop current bit
        bitboard &= bitboard - 1;
    }

}

bool Board::is_square_attacked_by_colour(int square, int colour, Board &board)
{
    Bitboard attack_squares;
    int side = colour == white ? 0 : 6;
    // attacked by rook
    attack_squares = generate_rook_attack_from_square(square, board.occupied_squares);
    if (attack_squares & board.piece_bitboards[R + side]) return true;

    // attacked by bishop
    attack_squares = generate_bishop_attack_from_square(square, board.occupied_squares);
    if (attack_squares & board.piece_bitboards[B + side]) return true;

    // attacked by queen
    attack_squares = generate_queen_attack_from_square(square, board.occupied_squares);
    if (attack_squares & board.piece_bitboards[Q + side]) return true;

    // attacked by knight
    attack_squares = KNIGHT_ATTACK_TABLE[square];
    if (attack_squares & board.piece_bitboards[N + side]) return true;

    // attacked by pawn
    attack_squares = PAWN_ATTACK_TABLE[colour ^ 1][square];
    if (attack_squares & board.piece_bitboards[P + side]) return true;

    // attacked by king
    attack_squares = KING_ATTACK_TABLE[square];
    if (attack_squares & board.piece_bitboards[K + side]) return true;

    return false;
}

void Board::generate_castling_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, Board &board)
{
    int side = board.game_state.side_to_move == white ? 0 : 6;
    int opponent = 1 ^ board.game_state.side_to_move;
    auto rights = board.game_state.side_to_move == white ? board.game_state.castling_rights >> 2 : board.game_state.castling_rights;

    Bitboard bitboard = board.piece_bitboards[K + side];
    while (bitboard)
    {
        int start_square = get_LS1B(bitboard);
        // kingside
        if (rights & 0b10)
        {
            if (!is_square_attacked_by_colour(start_square, opponent, board) &&
                !is_square_attacked_by_colour(start_square + 1, opponent, board) &&
                !get_bit(board.occupied_squares, start_square + 1) &&
                !get_bit(board.occupied_squares, start_square + 2))
            {
                move_list[move_count] = create_move(start_square, start_square + 2, 0b10);
                move_count++;
            }
        }
        // queenside
        if (rights & 0b1)
        {
            if (!is_square_attacked_by_colour(start_square, opponent, board) &&
                !is_square_attacked_by_colour(start_square - 1, opponent, board) &&
                !get_bit(board.occupied_squares, start_square - 1) &&
                !get_bit(board.occupied_squares, start_square - 2) &&
                !get_bit(board.occupied_squares, start_square - 3))
            {
                move_list[move_count] = create_move(start_square, start_square - 2, 0b11);
                move_count++;
            }
        }

        bitboard &= bitboard - 1;
    }
}

void Board::generate_pawn_pseudolegal_moves(std::vector<Move>& move_list, int& move_count, Board& board)
{
    int side = board.game_state.side_to_move == white ? 0 : 6;
    int offset = board.game_state.side_to_move == white ? 1 : -1;
    Bitboard& opponent_bitboard = board.game_state.side_to_move == white ? board.black_occupancy : board.white_occupancy;
    Bitboard promotion_mask = board.game_state.side_to_move == white ? RANK_8_MASK : RANK_1_MASK;
    Bitboard double_push_mask = board.game_state.side_to_move == white ? RANK_2_MASK : RANK_7_MASK;

    Bitboard bitboard = board.piece_bitboards[P + side];

    while (bitboard)
    {
        // get start square
        int start_square = get_LS1B(bitboard);
        Bitboard attack_squares = PAWN_ATTACK_TABLE[board.game_state.side_to_move][start_square];

        while (attack_squares)
        {
            int target_square = get_LS1B(attack_squares);
            if (get_bit(opponent_bitboard, target_square))
            {
                // promotion captures
                if (get_bit(promotion_mask, target_square))
                {
                    move_list[move_count] = create_move(start_square, target_square, 0b1100);
                    move_count++;
                    move_list[move_count] = create_move(start_square, target_square, 0b1101);
                    move_count++;
                    move_list[move_count] = create_move(start_square, target_square, 0b1110);
                    move_count++;
                    move_list[move_count] = create_move(start_square, target_square, 0b1111);
                    move_count++;
                }

                    // regular capture
                else
                {
                    move_list[move_count] = create_move(start_square, target_square, 0b100);
                    move_count++;
                }
            }

            // en-passant capture
            if (target_square == board.game_state.ep_square)
            {
                move_list[move_count] = create_move(start_square, board.game_state.ep_square, 0b101);
                move_count++;
            }
            attack_squares &= attack_squares - 1;
        }

        int single_push = start_square + (offset * 8);
        int double_push = start_square + (offset * 16);

        // double push
        if (get_bit(double_push_mask, start_square))
        {
            if (!get_bit(board.occupied_squares, single_push) &&
                !get_bit(board.occupied_squares, double_push))
            {
                move_list[move_count] = create_move(start_square, double_push, 0b1);
                move_count++;
            }
        }

        if (!get_bit(board.occupied_squares, single_push))
        {
            // single push promotion
            if (get_bit(promotion_mask, single_push))
            {
                move_list[move_count] = create_move(start_square, single_push, 0b1000);
                move_count++;
                move_list[move_count] = create_move(start_square, single_push, 0b1001);
                move_count++;
                move_list[move_count] = create_move(start_square, single_push, 0b1010);
                move_count++;
                move_list[move_count] = create_move(start_square, single_push, 0b1011);
                move_count++;
            }

                // single push
            else
            {
                move_list[move_count] = create_move(start_square, single_push, 0b0);
                move_count++;
            }
        }

        // pop first bit for next pawn
        bitboard &= bitboard - 1;
    }
}

std::pair<std::vector<Move>, int> Board::generate_all_pseudolegal_moves(Board &board) {
    // initialise return variables
    std::vector<Move> move_list(256);
    int move_count = 0;

    // generate rook moves
    generate_piece_pseudolegal_moves(move_list, move_count, board, R);

    // generate bishop moves
    generate_piece_pseudolegal_moves(move_list, move_count, board, B);

    // generate queen moves
    generate_piece_pseudolegal_moves(move_list, move_count, board, Q);

    // generate knight moves
    generate_piece_pseudolegal_moves(move_list, move_count, board, N);

    // generate king moves
    generate_piece_pseudolegal_moves(move_list, move_count, board, K);

    // castling
    generate_castling_pseudolegal_moves(move_list, move_count, board);

    // generate pawn moves
    generate_pawn_pseudolegal_moves(move_list, move_count, board);

    return std::make_pair(move_list, move_count);
}