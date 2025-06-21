
#include <iostream>

#include "board/Board.h"


int main()
{
    auto board = Board();
    board.parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
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
    board.move_piece(K, e1, e2);
    print(board.piece_bitboards[K]);
    std::cout << board.castling_rights << std::endl;
    board.update_castling_rights(h1, h3);
    std::cout << board.castling_rights << std::endl;
}
