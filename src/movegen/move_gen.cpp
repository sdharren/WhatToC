#include "move_gen.h"

#include "../helpers/bit_helpers.h"

Bitboard A_FILE_MASK = 0x0101010101010101;
Bitboard H_FILE_MASK = 0x8080808080808080;
Bitboard AB_FILE_MASK = 0x303030303030303;
Bitboard GH_FILE_MASK = 0xc0c0c0c0c0c0c0c0;

MoveGenerator::MoveGenerator()
{
    initialise_leaper_piece_attack_tables();
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

    // north
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

Bitboard MoveGenerator::generate_blocker_board(int index, int num_of_blockers, Bitboard attack_mask)
{
    Bitboard blocker_board = 0;
    for (int bit = 0; bit < num_of_blockers; bit++)
    {
        int square = get_LS1B(attack_mask);
        attack_mask &= attack_mask - 1;

        if (index & (1 << bit)) blocker_board |= 1ULL << square;
    }

    return blocker_board;
}
