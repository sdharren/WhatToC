#include "PRNG.h"
#include "ZobristRandoms.h"

ZobristRandoms::ZobristRandoms() : piece_randoms(12, std::vector<Bitboard>(64)), ep_square_randoms(65),
                                   castling_rights_randoms(16), side_to_move_randoms(2)
{
    init_piece_randoms();
    init_ep_square_randoms();
    init_castling_rights_randoms();
    init_side_to_move_randoms();
}

// Generate random numbers for pieces, on each square
void ZobristRandoms::init_piece_randoms()
{
    for (int piece = K; piece <= p; piece++)
    {
        for (int square = 0; square < 64; square++)
        {
            piece_randoms[piece][square] = get_random_64();
        }
    }
}

// Generate a random number for en-passant squares
void ZobristRandoms::init_ep_square_randoms()
{
    for (int square = 0; square < 65; square++)
    {
        ep_square_randoms[square] = get_random_64();
    }
}

// Generate a random number for each castling right combination
void ZobristRandoms::init_castling_rights_randoms()
{
    for (int right = 0; right < 16; right++)
    {
        castling_rights_randoms[right] = get_random_64();
    }
}

// Generate 2 keys for each side to play
void ZobristRandoms::init_side_to_move_randoms()
{
    for (int side = white; side <= black; side++)
    {
        side_to_move_randoms[side] = get_random_64();
    }
}

