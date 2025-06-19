#include "find_magics.h"
#include "../helpers/bit_helpers.h"
#include "move_gen.h"
#include "../helpers/prng.h"
#include <iostream>

unsigned long long generate_magic_number()
{
    return get_random_64() & get_random_64() & get_random_64();
}

// generate blocker board number {index} for the attack mask
Bitboard generate_blocker_board(int index, int num_of_blockers, Bitboard attack_mask)
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

Bitboard generate_rook_attack_map_with_blockers(int square, Bitboard blocker_board)
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

Bitboard generate_bishop_attack_map_with_blockers(int square, Bitboard blocker_board)
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

Bitboard find_magic_number(int square, int piece)
{
    // Initialise the correct attack mask
    auto x = MoveGenerator();
    Bitboard attack_mask = piece == R ? x.generate_rook_attack_mask_from_square(square) :
                                        x.generate_bishop_attack_mask_from_square(square);

    // blocker information
    auto num_of_possible_blockers = count_bits(attack_mask);
    auto total_num_of_blocker_boards = 1 << num_of_possible_blockers;

    // link blocker board to attack map
    std::array<Bitboard, 4096> blocker_boards{};
    std::array<Bitboard, 4096> attack_maps{};

    for (int index = 0; index < total_num_of_blocker_boards; index++)
    {
        blocker_boards[index] = generate_blocker_board(index, num_of_possible_blockers, attack_mask);
        attack_maps[index] = piece == R ? generate_rook_attack_map_with_blockers(square, blocker_boards[index]) :
                                          generate_bishop_attack_map_with_blockers(square, blocker_boards[index]);
    }

    std::array<Bitboard, 4096> used_attack_maps{};

    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        auto magic_number = generate_magic_number();

        // Reinitialise the used attack map
        used_attack_maps.fill(0ULL);

        // get rid of bad magic number candidates
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        int index, fail;
        for (index = 0, fail = 0; !fail && index < total_num_of_blocker_boards; index++)
        {
            // calculate magic index
            auto magic_index = (blocker_boards[index] * magic_number) >> (64 - num_of_possible_blockers);

            // if there is no attack map, then fill
            if (used_attack_maps[magic_index] == 0ULL)
            {
                used_attack_maps[magic_index] = attack_maps[index];
            }
            // if there is an attack map, check if its the correct one
            else if (used_attack_maps[magic_index] != attack_maps[index])
            {
                fail = 1;
            }
        }
        if (!fail)
        {
            return magic_number;
        }

    }
    return 0ULL;
}

void find_all_magic_numbers()
{
    std::array<Bitboard, 64> rook_magics;
    std::array<Bitboard, 64> bishop_magics;

    for (int square = 0; square < 64; square++)
    {
        rook_magics[square] = find_magic_number(square, R);
    }

    for (int square = 0; square < 64; square++)
    {
        bishop_magics[square] = find_magic_number(square, B);
    }

    std::cout << "rook magic numbers" << std::endl;
    for (auto n: rook_magics)
    {
        std::cout << std::hex << n << "ULL," << std::endl;
    }
    std::cout << "bishop magic numbers" << std::endl;
    for (auto n: bishop_magics)
    {
        std::cout << std::hex << n << "ULL," << std::endl;
    }
}