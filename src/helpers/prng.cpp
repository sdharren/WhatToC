#include "prng.h"

unsigned int RANDOM_STATE = 1804289383;

unsigned int get_random_32()
{
    auto number = RANDOM_STATE;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update RANDOM_STATE
    RANDOM_STATE = number;

    return number;
}

unsigned long long get_random_64()
{
    // generate 4 random 32-bit numbers, and get the first 16 bits
    auto n1 = static_cast<unsigned long long>(get_random_32()) & 0xffff;
    auto n2 = static_cast<unsigned long long>(get_random_32()) & 0xffff;
    auto n3 = static_cast<unsigned long long>(get_random_32()) & 0xffff;
    auto n4 = static_cast<unsigned long long>(get_random_32()) & 0xffff;

    // generate the full 64-bit number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}