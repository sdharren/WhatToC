
#include <iostream>

#include "helpers/bit_helpers.h"
#include "movegen/move_gen.h"

int main()
{
    auto x = MoveGenerator();
    auto y = x.generate_rook_attack_mask_from_square(0);
    auto z = count_bits(y);

    print(x.generate_blocker_board(4093, z, y));
}
