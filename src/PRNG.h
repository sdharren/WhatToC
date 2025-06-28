#ifndef PRNG_H
#define PRNG_H

// Random state variable
inline unsigned int RANDOM_STATE = 1804289383;

// PRNG functions
unsigned int get_random_32();
unsigned long long get_random_64();

#endif //PRNG_H
