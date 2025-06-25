#include "move_gen.h"

#include "find_magics.h"
#include "../helpers/bit_helpers.h"

Bitboard A_FILE_MASK = 0x0101010101010101;
Bitboard H_FILE_MASK = 0x8080808080808080;
Bitboard AB_FILE_MASK = 0x303030303030303;
Bitboard GH_FILE_MASK = 0xc0c0c0c0c0c0c0c0;

MoveGenerator::MoveGenerator() : PAWN_ATTACK_TABLE(2, std::vector<Bitboard>(64)), KNIGHT_ATTACK_TABLE(64),
                                 KING_ATTACK_TABLE(64), ROOK_ATTACK_TABLE(64, std::vector<Bitboard>(4096)),
                                 BISHOP_ATTACK_TABLE(64, std::vector<Bitboard>(512))
{
    initialise_leaper_piece_attack_tables();
    initialise_rook_attack_table();
    initialise_bishop_attack_table();
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

Bitboard MoveGenerator::generate_rook_attack_mask_from_square(int square)
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

Bitboard MoveGenerator::generate_bishop_attack_mask_from_square(int square)
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

void MoveGenerator::generate_piece_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, Board &board,
                                                      int piece)
{
    int side = board.side_to_move == white ? 0 : 6;
    Bitboard& player_bitboard = board.side_to_move == white ? board.white_occupancy : board.black_occupancy;
    Bitboard& opponent_bitboard = board.side_to_move == white ? board.black_occupancy : board.white_occupancy;

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

void MoveGenerator::generate_castling_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, Board &board)
{
    int side = board.side_to_move == white ? 0 : 6;
    int opponent = 1 ^ board.side_to_move;
    auto rights = board.side_to_move == white ? board.castling_rights >> 2 : board.castling_rights;

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

std::pair<std::vector<Move>, int> MoveGenerator::generate_all_pseudolegal_moves(Board &board)
{
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
    // captures
    // regular

    // promotion

    // ep

    // quiets
    // single push

    // double push

    // promotion

    return std::make_pair(move_list, move_count);
}