
#include <iostream>

#include "movegen/move_gen.h"

int main()
{
    auto x = MoveGenerator();
    std::cout << "hi" << std::endl;
    print(x.PAWN_ATTACK_TABLE[1][14]);
}