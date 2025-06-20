
#include <iostream>

#include "helpers/bit_helpers.h"
#include "movegen/move_gen.h"

int main()
{
    auto x = MoveGenerator();
    Bitboard blocker = 0;
    set_bit(blocker, 59);
    set_bit(blocker, 54);
    set_bit(blocker, 43);
    set_bit(blocker, 30);
    set_bit(blocker, 13);
    set_bit(blocker, 0);
    // print(blocker);
    // print(blocker);
    print(x.generate_rook_attack_from_square(27, blocker));
    print(x.generate_bishop_attack_from_square(27, blocker));
    print(x.generate_queen_attack_from_square(27, blocker));
}