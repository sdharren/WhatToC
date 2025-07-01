#include <chrono>
#include <iostream>

#include "PERFT.h"


void perft_tests();

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    perft_tests();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << diff.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    perft_tests();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << diff.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    perft_tests();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << diff.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    perft_tests();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << diff.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    perft_tests();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << diff.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    perft_tests();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << diff.count() << std::endl;
}