#pragma once

#include "permutations.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <bitset>
#include <cstdio>
#include <set>
#include <fstream>
#include <unordered_set>
#include "4poles.hpp"
#include "binaryfunctions.hpp"

namespace std {
    template<>
    struct hash<std::unordered_set<int>> {
        size_t operator()(const std::unordered_set<int> &v) const {
            size_t sum = 0;
            size_t size = 0;
            for (auto element: v) {
                sum += element;
                size += 1;
            }
            return sum + size;
        }
    };
}

bool isKempeClosed(long long int representation);

void determineAllPossibleRepresentants(std::ofstream &outdata);

long long int getRepresentant(long long int representation);

bool isRepresentant(long long int representation);

void initializeMasks();

long long int getPermutation(long long int representation, int permutation);