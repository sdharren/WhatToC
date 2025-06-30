#include "PERFT.h"

#include <iostream>
#include <ostream>

PERFT::PERFT() : engine(Engine()), nodes(0)
{

}

PERFT::PERFT(std::string FEN_string, int depth) : engine(Engine(FEN_string)), nodes(0)
{
    perft_driver(depth);
    std::cout << nodes << std::endl;
}

void PERFT::perft_driver(int depth)
{
    if (depth == 0)
    {
        nodes++;
        return;
    }

    auto moves = engine.move_generator.generate_all_pseudolegal_moves(engine.board);
    for (int i = 0; i < moves.second; i++)
    {
        auto copy_game_state = engine.board.game_state;
        int target_piece = engine.board.piece_list[get_target_square(moves.first[i])];

        if (!engine.make_move(moves.first[i])) continue;
        perft_driver(depth - 1);

        engine.unmake_move(moves.first[i], copy_game_state, target_piece);
    }
}

void PERFT::perft_test_driver(int depth)
{
    Board old_board = engine.board;
    if (depth == 0)
    {
        nodes++;
        return;
    }

    auto moves = engine.move_generator.generate_all_pseudolegal_moves(engine.board);
    for (int i = 0; i < moves.second; i++)
    {
        auto copy_game_state = engine.board.game_state;
        int target_piece = engine.board.piece_list[get_target_square(moves.first[i])];

        if (!engine.make_move(moves.first[i])) continue;

        int curr_nodes = nodes;
        perft_driver(depth - 1);
        int diff = nodes - curr_nodes;

        engine.unmake_move(moves.first[i], copy_game_state, target_piece);

        print_move(moves.first[i]);
        std::cout << diff << std::endl;
    }
}