#pragma once
#include <vector>
#include <fstream>

void permute(std::vector<int> &numbers, int length, int number);

long long int **initializePermutations();

void deletePermutation();

void counterToZero();

void getPermutation(std::vector<int> &numbers, int length, int permutation, std::ofstream &outdata);