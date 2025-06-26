//
// Created by Dharren on 26/06/2025.
//

#ifndef PERFT_H
#define PERFT_H
#include "../board/Board.h"
#include "../movegen/move_gen.h"


class PERFT {
public:
    Board board;
    MoveGenerator move_gen;
    int nodes;

    PERFT();
    void perft_driver(int depth);
};



#endif //PERFT_H
