#include <fstream>
#include <iostream>
#include <vector>
#include "disjuction.h"

class NumberReader {
    std::ifstream inData;
    bool hasNextNumber = true;

public:
    explicit NumberReader(const char *path) {
        inData.open(path);
        if (!inData) {
            std::cerr << "Error: File could not be opened" << std::endl;
        }
    }

    long long int next() {
        long long int number;
        if (inData >> number) {
            return number;
        } else {
            hasNextNumber = false;
            return -1;
        }
    }

    bool hasNext() const {
        return hasNextNumber;
    }

};

std::vector<long long int> getRepresentants(const char *path) {
    NumberReader numberReader = NumberReader(path);
    std::vector<long long int> numbers;
    while (numberReader.hasNext()) {
        long long int number = numberReader.next();
        if (number != -1) {
            numbers.emplace_back(number);
        }
    }
    return numbers;

}

