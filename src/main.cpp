
#include <iostream>

#include "helpers/bit_helpers.h"
#include "movegen/move_gen.h"

int main()
{
    auto x = MoveGenerator();
    auto y = x.generate_bishop_attack_mask_from_square(21);
    auto z = count_bits(y);

    auto a = x.generate_blocker_board(511, z, y);
    print(a);
    print(x.generate_bishop_attack_map_with_blockers(21, a));
}
