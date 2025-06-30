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

        if (!engine.make_move(moves.first[i]))
        {
            if (engine.board == old_board)
            {

            }
            else
            {
                print_move(moves.first[i]);
                std::cout << "illegal move shouldve returned this" << std::endl;
                engine.board.print_piece_list();
                std::cout << "shouldve done this" << std::endl;
                old_board.print_piece_list();
                // break;
            }
            continue;
        }

        int curr_nodes = nodes;
        ml.push_back(moves.first[i]);
        perft_test_driver(depth - 1);
        int diff = nodes - curr_nodes;

        engine.unmake_move(moves.first[i], copy_game_state, target_piece);
        if (engine.board == old_board)
        {

        }
        else
        {
            for (auto m: ml)
            {
                print_move(m);
            }
            std::cout << "depth is: " << depth << std::endl;
            std::cout << "move shouldve returned this" << std::endl;
            engine.board.print_piece_list();
            std::cout << "shouldve done this" << std::endl;
            old_board.print_piece_list();
            // break;
        }
        ml.pop_back();
        // print_move(moves.first[i]);
        // std::cout << diff << std::endl;
    }
}