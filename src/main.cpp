
#include <iostream>

#include "board/Board.h"
#include "movegen/move_gen.h"

int main()
{
    auto board = Board();
    board.parse_FEN("r1bqk2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1");
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
    auto move_gen = MoveGenerator();
    auto x = move_gen.generate_all_pseudolegal_moves(board);
    std::cout << x.second;
}
