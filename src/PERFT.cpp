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
        auto copy_piece_bitboards = engine.board.piece_bitboards;
        auto copy_white_occupancy = engine.board.white_occupancy;
        auto copy_black_occupancy = engine.board.black_occupancy;
        auto copy_occupied_squares = engine.board.occupied_squares;
        auto copy_piece_list = engine.board.piece_list;
        auto copy_game_state = engine.board.game_state;

        if (!engine.make_move(moves.first[i])) continue;
        perft_driver(depth - 1);

        engine.board.piece_bitboards = copy_piece_bitboards;
        engine.board.white_occupancy = copy_white_occupancy;
        engine.board.black_occupancy = copy_black_occupancy;
        engine.board.occupied_squares = copy_occupied_squares;
        engine.board.piece_list = copy_piece_list;
        engine.board.game_state = copy_game_state;
    }
}

void PERFT::perft_test_driver(int depth)
{
    if (depth == 0)
    {
        nodes++;
        return;
    }

    auto moves = engine.move_generator.generate_all_pseudolegal_moves(engine.board);
    for (int i = 0; i < moves.second; i++)
    {
        auto copy_piece_bitboards = engine.board.piece_bitboards;
        auto copy_white_occupancy = engine.board.white_occupancy;
        auto copy_black_occupancy = engine.board.black_occupancy;
        auto copy_occupied_squares = engine.board.occupied_squares;
        auto copy_piece_list = engine.board.piece_list;
        auto copy_game_state = engine.board.game_state;

        if (!engine.make_move(moves.first[i])) continue;

        int curr_nodes = nodes;
        perft_driver(depth - 1);
        int diff = nodes - curr_nodes;

        engine.board.piece_bitboards = copy_piece_bitboards;
        engine.board.white_occupancy = copy_white_occupancy;
        engine.board.black_occupancy = copy_black_occupancy;
        engine.board.occupied_squares = copy_occupied_squares;
        engine.board.piece_list = copy_piece_list;
        engine.board.game_state = copy_game_state;

        print_move(moves.first[i]);
        std::cout << diff << std::endl;
    }
}