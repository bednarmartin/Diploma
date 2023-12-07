#include <fstream>
#include "permutations.h"
#include "4poles.hpp"

int counter = 0;
long long int **array;

void getPermutation(std::vector<int> &numbers, int length, int permutation, std::ofstream &outdata) {
    if (length == 4) {
        if (counter == permutation) {
            outdata << numbers[0] << " " << numbers[1] << " " << numbers[2] << " " << numbers[3] << std::endl;
        }
        counter++;
    } else {
        for (int i = length; i <= 3; i++) {
            std::swap(numbers[length], numbers[i]);
            getPermutation(numbers, length + 1, permutation, outdata);
            std::swap(numbers[length], numbers[i]);
        }
    }
}

void counterToZero() {
    counter = 0;
}

void permute(std::vector<int> &numbers, int length, int number) {
    if (length == 4) {
        int output = determine(numbers[0], numbers[1], numbers[2], numbers[3]);
        long long int toInsert = 1ll << output;
        array[number][counter] = toInsert;
        counter++;
    } else {
        for (int i = length; i <= 3; i++) {
            std::swap(numbers[length], numbers[i]);
            permute(numbers, length + 1, number);
            std::swap(numbers[length], numbers[i]);
        }
    }
}

long long int **initializePermutations() {
    array = new long long int *[38];
    for (int i = 0; i < 38; ++i) {
        array[i] = new long long int[24];
    }
    for (int i = 0; i < 38; i++) {
        std::vector<int> example = getExample(i);
        permute(example, 0, i);
        counter = 0;
    }
    return array;
}

void deletePermutation() {
    for (int i = 0; i < 38; ++i) {
        delete[] array[i];
    }
    delete[] array;
}