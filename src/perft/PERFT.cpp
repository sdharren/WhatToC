//
// Created by Dharren on 26/06/2025.
//

#include "PERFT.h"

PERFT::PERFT() : nodes(0), board(Board()), move_gen(MoveGenerator())
{

}

void PERFT::perft_driver(int depth)
{
    if (depth == 0)
    {
        nodes++;
        return;
    }

    auto moves = move_gen.generate_all_pseudolegal_moves(board);
    for (int i = 0; i < moves.second; i++)
    {
        auto copy_piece_bitboards = board.piece_bitboards;
        auto copy_white_occupancy = board.white_occupancy;
        auto copy_black_occupancy = board.black_occupancy;
        auto copy_occupied_squares = board.occupied_squares;
        auto copy_piece_list = board.piece_list;
        auto copy_game_state = board.game_state;

        if (!board.make_move(moves.first[i])) continue;
        perft_driver(depth - 1);

        board.piece_bitboards = copy_piece_bitboards;
        board.white_occupancy = copy_white_occupancy;
        board.black_occupancy = copy_black_occupancy;
        board.occupied_squares = copy_occupied_squares;
        board.piece_list = copy_piece_list;
        board.game_state = copy_game_state;
    }
}
