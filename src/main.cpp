#include <chrono>
#include <iostream>

#include "PERFT.h"


void perft_tests();

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    perft_tests();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << diff.count() << std::endl;


    // auto p = PERFT();
    // p.engine.board.parse_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - 0 2");
    // p.perft_test_driver(2);
    // std::cout << p.nodes << std::endl;

    // auto perft = PERFT();
    // perft.engine.board.parse_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - 0 2");
    // auto standard_board = Board();
    // standard_board.parse_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - 0 2");
    //
    // auto moves = perft.engine.move_generator.generate_all_pseudolegal_moves(perft.engine.board);
    // std::cout << moves.second << std::endl;
    // for (int i = 0; i < moves.second; i++)
    // {
    //     auto gs = perft.engine.board.game_state;
    //     auto tp = perft.engine.board.piece_list[get_target_square(moves.first[i])];
    //     if (perft.engine.make_move(moves.first[i]))
    //     {
    //         perft.engine.unmake_move(moves.first[i], gs, tp);
    //     }
    //     if (perft.engine.board == standard_board)
    //     {
    //         print_move(moves.first[i]);
    //         std::cout << "returns the same board" << std::endl;
    //     }
    //     else
    //     {
    //         print_move(moves.first[i]);
    //         std::cout << "returns different board" << std::endl;
    //         break;
    //     }
    // }
    // perft.perft_test_driver(2);
    // std::cout << perft.nodes << std::endl;
    // print_move(moves.first[0]);
    // perft.engine.board.print_piece_list();
    // perft.engine.make_move(moves.first[0]);
    // std::cout << std::endl;
    // perft.engine.board.print_piece_list();
    // auto temp = create_move(e1, f1, 0);
    // perft.engine.make_move(temp);
    //
    // auto move = create_move(d5, d6, 0);
    // perft.engine.make_move(move);
    // // engine.board.print_piece_list();
    // perft.perft_driver(2);
    // std::cout << perft.nodes << std::endl;


}