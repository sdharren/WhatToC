
#include <iostream>

#include "helpers/bit_helpers.h"
#include "movegen/move_gen.h"

int main()
{
    auto x = MoveGenerator();
    print(x.generate_bishop_attack_mask_from_square(56));
}
