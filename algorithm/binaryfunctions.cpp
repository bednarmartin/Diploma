#include "binaryfunctions.hpp"
#include <vector>

long long int getCountOfOnes(long long int number) {
    long long int counter = 0;
    while (number != 0) {
        if (number & 1) {
            counter++;
        }
        number >>= 1;
    }
    return counter;
}

std::vector<long long int> getOnePositions(long long int number) {
    std::vector<long long int> positions;
    long long int counter = 0;
    while (number != 0) {
        if (number & 1) {
            positions.emplace_back(counter);
        }
        number >>= 1;
        counter++;
    }
    return positions;
}

long long int getOnes(long long int number) {
    long long int output = 0;
    for (int i = 0; i < number; i++) {
        output |= (1 << i);
    }
    return output;
}