#ifndef ENGINE_H
#define ENGINE_H
#include "Board.h"
#include "MoveGenerator.h"

class Engine {
public:
    Board board;
    MoveGenerator move_generator;


    Engine();
    explicit Engine(std::string FEN_string);

    bool make_move(Move move);
};



#endif //ENGINE_H
