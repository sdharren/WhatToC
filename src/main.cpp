
#include <iostream>

#include "board/Board.h"


int main()
{
    auto board = Board();
    board.parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 17 45");
    for (int i = K; i <= p; i++)
    {
        print(board.piece_bitboards[i]);
    }
    std::cout << board.side_to_move << std::endl;
    std::cout << board.castling_rights << std::endl;
    std::cout << board.ep_square << std::endl;
    std::cout << board.halfmove_clock << std::endl;
    std::cout << board.fullmove_counter << std::endl;
}
