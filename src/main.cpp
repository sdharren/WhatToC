#include <chrono>
#include <iostream>

void perft_tests();

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    perft_tests();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << diff.count() << std::endl;
    // std::vector<int> x(2);
    // x[1] = 2;

}