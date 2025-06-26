
#include <iostream>

#include "board/Board.h"
#include "movegen/move_gen.h"
#include "perft/PERFT.h"

int main()
{
    // auto board = Board();
    // board.parse_FEN("3r4/8/8/8/8/8/3B4/3K4 w - - 0 1");
    // for (int i = K; i <= p; i++)
    // {
    //     print(board.piece_bitboards[i]);
    // }
    // std::cout << board.side_to_move << std::endl;
    // std::cout << board.castling_rights << std::endl;
    // std::cout << board.ep_square << std::endl;
    // std::cout << board.halfmove_clock << std::endl;
    // std::cout << board.fullmove_counter << std::endl;
    // int j = 0;
    // for (auto n: board.piece_list)
    // {
    //     std::cout << n << " ";
    //     j++;
    //     if (j % 8 == 0)
    //     {
    //         j = 0;
    //         std::cout << std::endl;
    //     }
    // }
//    board.move_piece(K, e1, e2);
//    print(board.piece_bitboards[K]);
//    std::cout << board.castling_rights << std::endl;
//    board.update_castling_rights(h1, h3);
//    std::cout << board.castling_rights << std::endl;
    // auto move_gen = MoveGenerator();
    // auto x = move_gen.generate_all_pseudolegal_moves(board);
    // std::cout << x.second;
    // auto move = create_move(d2, c3, 0b0);
    // board.make_move(move);
    // print(board.piece_bitboards[r]);
    // print(board.piece_bitboards[B]);
    // print(board.piece_bitboards[K]);
    auto perft = PERFT();
    perft.board.parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft.perft_driver(2);
    std::cout << perft.nodes << std::endl;
}
