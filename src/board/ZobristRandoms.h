#ifndef ZOBRISTRANDOMS_H
#define ZOBRISTRANDOMS_H
#include <vector>
#include "../helpers/board_defs.h"


class ZobristRandoms {
public:
    std::vector<std::vector<Bitboard> > piece_randoms;
    std::vector<Bitboard> ep_square_randoms;
    std::vector<Bitboard> castling_rights_randoms;
    std::vector<Bitboard> side_to_move_randoms;

    ZobristRandoms();
    void init_piece_randoms();
    void init_ep_square_randoms();
    void init_castling_rights_randoms();
    void init_side_to_move_randoms();
};



#endif //ZOBRISTRANDOMS_H
