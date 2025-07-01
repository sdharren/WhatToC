#include "MoveGenerator.h"

MoveGenerator::MoveGenerator() : PAWN_ATTACK_TABLE(2, std::vector<Bitboard>(64)), KNIGHT_ATTACK_TABLE(64),
                                 KING_ATTACK_TABLE(64), ROOK_ATTACK_TABLE(64, std::vector<Bitboard>(4096)),
                                 BISHOP_ATTACK_TABLE(64, std::vector<Bitboard>(512))
{
    initialise_leaper_piece_attack_tables();
    initialise_rook_attack_table();
    initialise_bishop_attack_table();
}

/* -------------------------------------------------------------------------------------------------------------------
 * Populating attack tables, and generating attacks from each piece from a square
 */

void MoveGenerator::initialise_leaper_piece_attack_tables()
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

Bitboard MoveGenerator::generate_king_attack_from_square(int square)
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

Bitboard MoveGenerator::generate_knight_attack_from_square(int square)
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

Bitboard MoveGenerator::generate_pawn_attack_from_square(int square, int colour)
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

void MoveGenerator::initialise_rook_attack_table()
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

Bitboard MoveGenerator::generate_rook_attack_map_with_blockers(int square, Bitboard blocker_board)
{
    Bitboard attack_map = 0;
    int rank =  square / 8;
    int file = square % 8;
    int newSquare;

    // east
    for (int newFile = file + 1; newFile < 8; newFile++)
    {
        newSquare = rank * 8 + newFile;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    // west
    for (int newFile = file - 1; newFile >= 0; newFile--)
    {
        newSquare = rank * 8 + newFile;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    // north
    for (int newRank = rank + 1; newRank < 8; newRank++)
    {
        newSquare = newRank * 8 + file;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    // south
    for (int newRank = rank - 1; newRank >= 0; newRank--)
    {
        newSquare = newRank * 8 + file;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    return attack_map;
}

void MoveGenerator::initialise_bishop_attack_table()
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

Bitboard MoveGenerator::generate_bishop_attack_map_with_blockers(int square, Bitboard blocker_board)
{
    Bitboard attack_map = 0;
    int rank =  square / 8;
    int file = square % 8;
    int newSquare;

    // NE
    for (int newFile = file + 1, newRank = rank + 1; newFile < 8 && newRank < 8; newFile++, newRank++)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    // SE
    for (int newFile = file + 1, newRank = rank - 1; newFile < 8 && newRank >= 0; newFile++, newRank--)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    // SW
    for (int newFile = file - 1, newRank = rank - 1; newFile >= 0 && newRank >= 0; newFile--, newRank--)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    // NW
    for (int newFile = file - 1, newRank = rank + 1; newFile >= 0 && newRank < 8; newFile--, newRank++)
    {
        newSquare = newRank * 8 + newFile;
        set_bit(attack_map, newSquare);
        if (blocker_board & (1ULL << newSquare)) break;
    }

    return attack_map;
}

Bitboard MoveGenerator::generate_blocker_board(int index, int num_of_blockers, Bitboard attack_mask)
{
    Bitboard blocker_board = 0;
    for (int bit = 0; bit < num_of_blockers; bit++)
    {
        int square = __builtin_ctzll(attack_mask);
        attack_mask &= attack_mask - 1;

        if (index & (1 << bit)) blocker_board |= 1ULL << square;
    }

    return blocker_board;
}

// Rook, Bishop, and Queen need their own functions to retrieve attacks
Bitboard MoveGenerator::generate_rook_attack_from_square(int square, Bitboard occupancy)
{
    auto mask = ROOK_ATTACK_MASKS[square];
    mask &= occupancy;
    int magic_index = (mask * ROOK_MAGICS[square]) >> (64 - ROOK_SHIFT_BITS[square]);
    return ROOK_ATTACK_TABLE[square][magic_index];
}

Bitboard MoveGenerator::generate_bishop_attack_from_square(int square, Bitboard occupancy)
{
    auto mask = BISHOP_ATTACK_MASKS[square];
    mask &= occupancy;
    int magic_index = (mask * BISHOP_MAGICS[square]) >> (64 - BISHOP_SHIFT_BITS[square]);
    return BISHOP_ATTACK_TABLE[square][magic_index];
}

Bitboard MoveGenerator::generate_queen_attack_from_square(int square, Bitboard occupancy)
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

/* --------------------------------------------------------------------------------------------------------------------
 * These functions concern generating all pseudolegal moves for the board for a given ruleset
 */

std::pair<std::vector<Move>, int> MoveGenerator::generate_all_pseudolegal_moves(Board &board)
{
    // initialise return variables
    std::vector<Move> move_list(256);
    int move_count = 0;

    // generate rook moves
    generate_piece_pseudolegal_moves(move_list, move_count, R, board);

    // generate bishop moves
    generate_piece_pseudolegal_moves(move_list, move_count, B, board);

    // generate queen moves
    generate_piece_pseudolegal_moves(move_list, move_count, Q, board);

    // generate knight moves
    generate_piece_pseudolegal_moves(move_list, move_count, N, board);

    // generate king moves
    generate_piece_pseudolegal_moves(move_list, move_count, K, board);

    // castling
    generate_castling_pseudolegal_moves(move_list, move_count, board);

    // generate pawn moves
    generate_pawn_pseudolegal_moves(move_list, move_count, board);

    return std::make_pair(move_list, move_count);
}

void MoveGenerator::generate_piece_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, int piece,
                                                     Board &board)
{
    int side = board.game_state.side_to_move == white ? 0 : 6;
    Bitboard& player_bitboard = board.game_state.side_to_move == white ? board.white_occupancy : board.black_occupancy;
    Bitboard& opponent_bitboard = board.game_state.side_to_move == white ? board.black_occupancy : board.white_occupancy;

    Bitboard bitboard = board.piece_bitboards[piece + side];
    while (bitboard)
    {
        // get start square
        int start_square = __builtin_ctzll(bitboard);
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
            int target_square = __builtin_ctzll(attack_squares);

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

void MoveGenerator::generate_castling_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, Board &board)
{
    int side = board.game_state.side_to_move == white ? 0 : 6;
    int opponent = 1 ^ board.game_state.side_to_move;
    auto rights = board.game_state.side_to_move == white ? board.game_state.castling_rights >> 2 : board.game_state.castling_rights;

    Bitboard bitboard = board.piece_bitboards[K + side];
    while (bitboard)
    {
        int start_square = __builtin_ctzll(bitboard);
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

void MoveGenerator::generate_pawn_pseudolegal_moves(std::vector<Move>& move_list, int& move_count, Board &board)
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
        int start_square = __builtin_ctzll(bitboard);
        Bitboard attack_squares = PAWN_ATTACK_TABLE[board.game_state.side_to_move][start_square];

        while (attack_squares)
        {
            int target_square = __builtin_ctzll(attack_squares);
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

bool MoveGenerator::is_square_attacked_by_colour(int square, int colour, Board &board)
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