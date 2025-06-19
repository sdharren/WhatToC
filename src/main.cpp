
#include <iostream>

#include "helpers/bit_helpers.h"
#include "movegen/move_gen.h"

int main()
{
    Bitboard board = 16;
    std::cout << count_bits(board) << std::endl;
    std::cout << get_LS1B(board) << std::endl;
}
