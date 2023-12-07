#include "kempeswitches.h"

std::unordered_set<std::unordered_set<int>> masks2;
std::vector<std::vector<long long int>> secondTypeMasks;
std::vector<long long int> firstTypeMasks;

long long int **permutations = initializePermutations();

std::unordered_set<std::unordered_set<int>> kempeSwitches2(int number) {
    std::unordered_set<std::unordered_set<int>> numCom;
    for (int start = 0; start < 4; start++) {
        for (int end = start + 1; end < 4; end++) {
            for (int start2 = 0; start2 < 4; start2++) {
                for (int end2 = start2 + 1; end2 < 4; end2++) {
                    std::vector<int> example = getExample(number);
                    example[start] ^= (1 << 0);
                    example[start] ^= (1 << 3);
                    example[end] ^= (1 << 0);
                    example[end] ^= (1 << 3);
                    int output = determine(example[0], example[1], example[2], example[3]);

                    example = getExample(number);
                    example[start2] ^= (1 << 1);
                    example[start2] ^= (1 << 2);
                    example[end2] ^= (1 << 1);
                    example[end2] ^= (1 << 2);
                    int output2 = determine(example[0], example[1], example[2], example[3]);

                    example = getExample(number);
                    example[start] ^= (1 << 0);
                    example[start] ^= (1 << 3);
                    example[end] ^= (1 << 0);
                    example[end] ^= (1 << 3);
                    example[start2] ^= (1 << 1);
                    example[start2] ^= (1 << 2);
                    example[end2] ^= (1 << 1);
                    example[end2] ^= (1 << 2);

                    int output3 = determine(example[0], example[1], example[2], example[3]);
                    std::unordered_set<int> nums;
                    nums.insert(number);
                    nums.insert(output);
                    nums.insert(output2);
                    nums.insert(output3);
                    masks2.insert(nums);
                    numCom.insert(nums);

                }
            }
        }
    }
    return numCom;
}

inline std::unordered_set<std::unordered_set<int>> kempeSwitches3(int number) {
    std::unordered_set<std::unordered_set<int>> numCom;
    for (int start = 0; start < 4; start++) {
        for (int end = start + 1; end < 4; end++) {
            for (int start2 = 0; start2 < 4; start2++) {
                for (int end2 = start2 + 1; end2 < 4; end2++) {
                    std::vector<int> example = getExample(number);
                    example[start] ^= (1 << 0);
                    example[start] ^= (1 << 1);
                    example[end] ^= (1 << 0);
                    example[end] ^= (1 << 1);
                    int output = determine(example[0], example[1], example[2], example[3]);

                    example = getExample(number);
                    example[start2] ^= (1 << 2);
                    example[start2] ^= (1 << 3);
                    example[end2] ^= (1 << 2);
                    example[end2] ^= (1 << 3);
                    int output2 = determine(example[0], example[1], example[2], example[3]);

                    example = getExample(number);
                    example[start] ^= (1 << 0);
                    example[start] ^= (1 << 1);
                    example[end] ^= (1 << 0);
                    example[end] ^= (1 << 1);
                    example[start2] ^= (1 << 2);
                    example[start2] ^= (1 << 3);
                    example[end2] ^= (1 << 2);
                    example[end2] ^= (1 << 3);

                    int output3 = determine(example[0], example[1], example[2], example[3]);
                    std::unordered_set<int> nums;
                    nums.insert(number);
                    nums.insert(output);
                    nums.insert(output2);
                    nums.insert(output3);
                    masks2.insert(nums);
                    numCom.insert(nums);

                }
            }
        }
    }
    return numCom;
}

inline std::unordered_set<int> kempeSwitches(int number) {
    std::unordered_set<int> numbers;
    for (int a = 0; a < 4; a++) {
        for (int b = a + 1; b < 4; b++) {
            for (int start = 0; start < 4; start++) {
                for (int end = start + 1; end < 4; end++) {
                    std::vector<int> example = getExample(number);
                    if (((((example[start] >> a) ^ (example[start] >> b)) & 1) == 1) &&
                        ((((example[end] >> a) ^ (example[end] >> b)) & 1) == 1)) {
                        example[start] ^= (1 << a);
                        example[start] ^= (1 << b);
                        example[end] ^= (1 << a);
                        example[end] ^= (1 << b);
                        int output = determine(example[0], example[1], example[2], example[3]);
                        numbers.insert(output);
                    }

                }
            }
        }
    }
    return numbers;
}

bool isKempeClosed(long long int representation) {
    int position = 0;
    long long int positionBinary = 1;
    for (auto mask: firstTypeMasks) {
        if (getCountOfOnes(representation & mask) == 1) {
            return false;
        }
    }

    long long int secondTypePossibilities = 0b11110000000111111000000000000000000000 & representation;
    long long int copyOfSecondTypePossibilities = secondTypePossibilities;

    while (secondTypePossibilities != 0 && position < 38) {
        if (secondTypePossibilities & positionBinary) {
            bool isGood = false;
            for (auto mask: secondTypeMasks[position]) {
                if ((mask & copyOfSecondTypePossibilities) == mask) {
                    secondTypePossibilities = (secondTypePossibilities ^ mask) & secondTypePossibilities;
                    isGood = true;
                    break;
                }
            }
            if (!isGood) {
                return false;
            }
        }
        position++;
        positionBinary <<= 1ll;
    }
    return secondTypePossibilities == 0;
}

bool isRepresentant(long long int representation) {
    for (int perm = 1; perm < 24; perm++) {
        long long int copy_representation = representation;
        long long int new_representation = 0;
        for (int pos = 0; pos < 38; pos++) {
            if (copy_representation & 1) {
                new_representation = new_representation | permutations[pos][perm];
            }
            copy_representation >>= 1ll;
        }
        if (new_representation < representation) {
            return false;
        }
    }
    return true;
}

long long int getRepresentant(long long int representation) {
    long long int lowestSoFar = representation;
    //int lowestperm;
    for (int perm = 0; perm < 24; perm++) {
        long long int copy_representation = representation;
        long long int new_representation = 0;
        for (int pos = 0; pos < 38; pos++) {
            if (copy_representation & 1) {
                new_representation = new_representation | permutations[pos][perm];
            }
            copy_representation >>= 1ll;
        }
        /*if (new_representation == 274854811748) {
            std::cout << "TU SOM " << new_representation << std::endl;
            lowestSoFar = new_representation;
            int lowestperm = perm;
            std::cout << "GOOD PERM = " << lowestperm << std::endl;
            std::vector<int> numbers = {0,1,2,3};
            counterToZero();
            getPermutation(numbers, 0, lowestperm);
        }*/

        if (new_representation < lowestSoFar) {
            lowestSoFar = new_representation;
        }
    }
    //std::cout << "GOOD PERM = " << lowestperm << std::endl;
    //std::vector<int> numbers = {0, 1, 2, 3};
    //counterToZero();
    //getPermutation(numbers, 0, lowestperm);
    //std::cout << std::endl;
    return lowestSoFar;
}

long long int getPermutation(long long int representation, int permutation) {
    if (permutation < 0 || permutation > 24) {
        return -1;
    }
    long long int copy_representation = representation;
    long long int new_representation = 0;
    for (int pos = 0; pos < 38; pos++) {
        if (copy_representation & 1) {
            new_representation = new_representation | permutations[pos][permutation];
        }
        copy_representation >>= 1ll;
    }
    return new_representation;
}


void initializeMasks() {
    secondTypeMasks = std::vector<std::vector<long long int>>(38);
    std::unordered_set<std::unordered_set<int>> masks1;
    for (int i = 0; i < 21; i++) {
        std::unordered_set<int> numbersType2 = kempeSwitches(i);
        masks1.insert(numbersType2);
    }

    for (int i = 28; i < 34; i++) {
        std::unordered_set<int> numbersType2 = kempeSwitches(i);
        masks1.insert(numbersType2);
    }

    for (const auto &mask: masks1) {
        long long int numMask = 0;
        for (auto num: mask) {
            numMask = numMask | (1ll << num);
        }
        firstTypeMasks.emplace_back(numMask);
    }

    for (int i = 21; i < 27; i++) {
        std::unordered_set<std::unordered_set<int>> numCom = kempeSwitches3(i);
        std::vector<long long int> masks;
        for (const auto &com_set: numCom) {
            long long int numMask = 0;
            for (auto com: com_set) {
                numMask = numMask | (1ll << com);
            }
            masks.emplace_back(numMask);
        }
        secondTypeMasks[i] = masks;

    }
    for (int i = 34; i < 38; i++) {
        std::unordered_set<std::unordered_set<int>> numCom = kempeSwitches2(i);
        std::vector<long long int> masks;
        for (const auto &com_set: numCom) {
            long long int numMask = 0;
            for (auto com: com_set) {
                numMask = numMask | (1ll << com);
            }
            masks.emplace_back(numMask);
        }
        secondTypeMasks[i] = masks;
    }
}

void determineAllPossibleRepresentants(std::ofstream &outdata) {
    initializeMasks();
    long long int counter = 0;
    for (long long int i = 0; i <= 0b11111111111111111111111111111111111111; i++) {
        if (i % 1000000000 == 0) {
            std::cout << 0b11111111111111111111111111111111111111 - i << " LEFT" << std::endl;
            std::cout << "COUNTER = " << counter << std::endl;
        }
        if (!isKempeClosed(i)) {
            continue;
        }
        if (!isRepresentant(i)) {
            continue;
        }
        counter++;
        outdata << i << " -" << std::endl;

    }
    std::cout << "FINAL COUNTER = " << counter << std::endl;
}

