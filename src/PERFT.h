#ifndef PERFT_H
#define PERFT_H
#include "Engine.h"

class PERFT {
public:
    Engine engine;
    int nodes;
    std::vector<Move> ml;

    PERFT();
    PERFT(std::string FEN_string, int depth);

    void perft_driver(int depth);
    void perft_test_driver(int depth);
};



#endif //PERFT_H
