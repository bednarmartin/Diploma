#include "4poles.hpp"
#include "binaryfunctions.hpp"
#include <vector>
#include <bitset>

bool hasA(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if (firstBit & secondBit & thirdBit & fourthBit == 1)
            return true;
    }
    return false;
}

bool hasT1(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        for (int j = i + 1; j < 4; j++) {
            int firstBit2 = (first >> j) & 1;
            int secondBit2 = (second >> j) & 1;
            int thirdBit2 = (third >> j) & 1;
            int fourthBit2 = (fourth >> j) & 1;
            if (firstBit == secondBit && thirdBit == fourthBit && thirdBit == !firstBit && firstBit2 == secondBit2 &&
                thirdBit2 == fourthBit2 &&
                firstBit != firstBit2 && thirdBit != thirdBit2) {
                return true;
            }
        }
    }
    return false;
}

bool hasT2(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        for (int j = i + 1; j < 4; j++) {
            int firstBit2 = (first >> j) & 1;
            int secondBit2 = (second >> j) & 1;
            int thirdBit2 = (third >> j) & 1;
            int fourthBit2 = (fourth >> j) & 1;
            if (firstBit == thirdBit && secondBit == fourthBit && secondBit == !firstBit && firstBit2 == thirdBit2 &&
                secondBit2 == fourthBit2 &&
                firstBit != firstBit2 && secondBit != secondBit2) {
                return true;
            }
        }
    }
    return false;
}

bool hasT3(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        for (int j = i + 1; j < 4; j++) {
            int firstBit2 = (first >> j) & 1;
            int secondBit2 = (second >> j) & 1;
            int thirdBit2 = (third >> j) & 1;
            int fourthBit2 = (fourth >> j) & 1;
            if (firstBit == fourthBit && secondBit == thirdBit && secondBit == !firstBit && firstBit2 == fourthBit2 &&
                secondBit2 == thirdBit2 &&
                firstBit != firstBit2 && secondBit != secondBit2) {
                return true;
            }
        }
    }
    return false;
}

bool hasN(int first, int second, int third, int fourth) {
    return getCountOfOnes(first) == 1 && getCountOfOnes(second) == 1 && getCountOfOnes(third) == 1 &&
           getCountOfOnes(fourth) == 1;
}

bool hasP1(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if ((firstBit & secondBit) == 1 && (thirdBit & fourthBit) == 0 && getCountOfOnes(third) == 1 &&
            getCountOfOnes(fourth) == 1) {
            return true;
        }
    }
    return false;
}

bool hasP2(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if ((firstBit & thirdBit) == 1 && (secondBit & fourthBit) == 0 && getCountOfOnes(second) == 1 &&
            getCountOfOnes(fourth) == 1) {
            return true;
        }
    }
    return false;
}

bool hasP3(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if ((firstBit & fourthBit) == 1 && (secondBit & thirdBit) == 0 && getCountOfOnes(second) == 1 &&
            getCountOfOnes(third) == 1) {
            return true;
        }
    }
    return false;
}

bool hasP4(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if ((secondBit & thirdBit) == 1 && (firstBit & fourthBit) == 0 && getCountOfOnes(first) == 1 &&
            getCountOfOnes(fourth) == 1) {
            return true;
        }
    }
    return false;
}

bool hasP5(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if ((secondBit & fourthBit) == 1 && (firstBit & thirdBit) == 0 && getCountOfOnes(first) == 1 &&
            getCountOfOnes(third) == 1) {
            return true;
        }
    }
    return false;
}

bool hasP6(int first, int second, int third, int fourth) {
    for (int i = 0; i < 4; i++) {
        int firstBit = (first >> i) & 1;
        int secondBit = (second >> i) & 1;
        int thirdBit = (third >> i) & 1;
        int fourthBit = (fourth >> i) & 1;
        if ((thirdBit & fourthBit) == 1 && (firstBit & secondBit) == 0 && getCountOfOnes(first) == 1 &&
            getCountOfOnes(second) == 1) {
            return true;
        }
    }
    return false;
}

int determine(int first, int second, int third, int fourth) {
    if (hasN(first, second, third, fourth)) {
        if (hasA(first, second, third, fourth)) {
            return AN;
        }
        if (hasT1(first, second, third, fourth)) {
            return T1N;
        }
        if (hasT2(first, second, third, fourth)) {
            return T2N;
        }
        if (hasT3(first, second, third, fourth)) {
            return T3N;
        }
    } else {
        if (hasT1(first, second, third, fourth)) {
            if (hasP1(first, second, third, fourth)) {
                return T1P1;
            }
            if (hasP2(first, second, third, fourth)) {
                return T1P2;
            }
            if (hasP3(first, second, third, fourth)) {
                return T1P3;
            }
            if (hasP4(first, second, third, fourth)) {
                return T1P4;
            }
            if (hasP5(first, second, third, fourth)) {
                return T1P5;
            }
            if (hasP6(first, second, third, fourth)) {
                return T1P6;
            }
            if (hasT2(first, second, third, fourth)) {
                return T1T2;
            }
            if (hasT3(first, second, third, fourth)) {
                return T1T3;
            }
            if (hasA(first, second, third, fourth)) {
                return AT1;
            }
            return T1T1;
        }
        if (hasT2(first, second, third, fourth)) {
            if (hasP1(first, second, third, fourth)) {
                return T2P1;
            }
            if (hasP2(first, second, third, fourth)) {
                return T2P2;
            }
            if (hasP3(first, second, third, fourth)) {
                return T2P3;
            }
            if (hasP4(first, second, third, fourth)) {
                return T2P4;
            }
            if (hasP5(first, second, third, fourth)) {
                return T2P5;
            }
            if (hasP6(first, second, third, fourth)) {
                return T2P6;
            }
            if (hasT3(first, second, third, fourth)) {
                return T2T3;
            }
            if (hasA(first, second, third, fourth)) {
                return AT2;
            }
            return T2T2;
        }
        if (hasT3(first, second, third, fourth)) {
            if (hasP1(first, second, third, fourth)) {
                return T3P1;
            }
            if (hasP2(first, second, third, fourth)) {
                return T3P2;
            }
            if (hasP3(first, second, third, fourth)) {
                return T3P3;
            }
            if (hasP4(first, second, third, fourth)) {
                return T3P4;
            }
            if (hasP5(first, second, third, fourth)) {
                return T3P5;
            }
            if (hasP6(first, second, third, fourth)) {
                return T3P6;
            }
            if (hasA(first, second, third, fourth)) {
                return AT3;
            }
            return T3T3;
        }
        if (hasA(first, second, third, fourth)) {
            if (hasP1(first, second, third, fourth)) {
                return AP1;
            }
            if (hasP2(first, second, third, fourth)) {
                return AP2;
            }
            if (hasP3(first, second, third, fourth)) {
                return AP3;
            }
            if (hasP4(first, second, third, fourth)) {
                return AP4;
            }
            if (hasP5(first, second, third, fourth)) {
                return AP5;
            }
            if (hasP6(first, second, third, fourth)) {
                return AP6;
            }
            return AA;
        }
    }
    return -1;
}

std::string convert(long long int number) {
    switch (number) {
        case T1N:
            return "T1N";
        case T2N:
            return "T2N";
        case T3N:
            return "T3N";
        case T1P1:
            return "T1P1";
        case T1P2:
            return "T1P2";
        case T1P3:
            return "T1P3";
        case T1P4:
            return "T1P4";
        case T1P5:
            return "T1P5";
        case T1P6:
            return "T1P6";
        case T2P1:
            return "T2P1";
        case T2P2:
            return "T2P2";
        case T2P3:
            return "T2P3";
        case T2P4:
            return "T2P4";
        case T2P5:
            return "T2P5";
        case T2P6:
            return "T2P6";
        case T3P1:
            return "T3P1";
        case T3P2:
            return "T3P2";
        case T3P3:
            return "T3P3";
        case T3P4:
            return "T3P4";
        case T3P5:
            return "T3P5";
        case T3P6:
            return "T3P6";
        case T1T1:
            return "T1T1";
        case T1T2:
            return "T1T2";
        case T1T3:
            return "T1T3";
        case T2T2:
            return "T2T2";
        case T2T3:
            return "T2T3";
        case T3T3:
            return "T3T3";
        case AN:
            return "AN";
        case AP1:
            return "AP1";
        case AP2:
            return "AP2";
        case AP3:
            return "AP3";
        case AP4:
            return "AP4";
        case AP5:
            return "AP5";
        case AP6:
            return "AP6";
        case AT1:
            return "AT1";
        case AT2:
            return "AT2";
        case AT3:
            return "AT3";
        case AA:
            return "AA";
        default:
            return "ERROR";


    }
}

std::string convertRepresentation(long long int representation) {
    long long int copyOfRepresentation = representation;
    std::string output;
    for (int position = 0; position < 38; position++) {
        if (copyOfRepresentation & 1) {
            output.append(convert(position));
            output.append(" ");
        }
        copyOfRepresentation >>= 1ll;
    }
    return output;
}

std::vector<int> getExample(int number) {
    int first, second, third, fourth;
    switch (number) {
        case T1N:
            first = 0b0001;
            second = 0b0001;
            third = 0b0010;
            fourth = 0b0010;
            break;
        case T2N:
            first = 0b0001;
            second = 0b0010;
            third = 0b0001;
            fourth = 0b0010;
            break;
        case T3N:
            first = 0b0001;
            second = 0b0010;
            third = 0b0010;
            fourth = 0b0001;
            break;
        case T1P1:
            first = 0b0101;
            second = 0b0101;
            third = 0b0010;
            fourth = 0b0010;
            break;
        case T1P2:
            first = 0b0101;
            second = 0b0001;
            third = 0b0110;
            fourth = 0b0010;
            break;
        case T1P3:
            first = 0b0101;
            second = 0b0001;
            third = 0b0010;
            fourth = 0b0110;
            break;
        case T1P4:
            first = 0b0001;
            second = 0b0101;
            third = 0b0110;
            fourth = 0b0010;
            break;
        case T1P5:
            first = 0b0001;
            second = 0b0101;
            third = 0b0010;
            fourth = 0b0110;
            break;
        case T1P6:
            first = 0b0001;
            second = 0b0001;
            third = 0b0110;
            fourth = 0b0110;
            break;
        case T2P1:
            first = 0b0101;
            second = 0b0110;
            third = 0b0001;
            fourth = 0b0010;
            break;
        case T2P2:
            first = 0b0101;
            second = 0b0010;
            third = 0b0101;
            fourth = 0b0010;
            break;
        case T2P3:
            first = 0b0101;
            second = 0b0010;
            third = 0b0001;
            fourth = 0b0110;
            break;
        case T2P4:
            first = 0b0001;
            second = 0b0110;
            third = 0b0101;
            fourth = 0b0010;
            break;
        case T2P5:
            first = 0b0001;
            second = 0b0110;
            third = 0b0001;
            fourth = 0b0110;
            break;
        case T2P6:
            first = 0b0001;
            second = 0b0010;
            third = 0b0101;
            fourth = 0b0110;
            break;
        case T3P1:
            first = 0b0101;
            second = 0b0110;
            third = 0b0010;
            fourth = 0b0001;
            break;
        case T3P2:
            first = 0b0101;
            second = 0b0010;
            third = 0b0110;
            fourth = 0b0001;
            break;
        case T3P3:
            first = 0b0101;
            second = 0b0010;
            third = 0b0010;
            fourth = 0b0101;
            break;
        case T3P4:
            first = 0b0001;
            second = 0b0110;
            third = 0b0110;
            fourth = 0b0001;
            break;
        case T3P5:
            first = 0b0001;
            second = 0b0110;
            third = 0b0010;
            fourth = 0b0101;
            break;
        case T3P6:
            first = 0b0001;
            second = 0b0010;
            third = 0b0110;
            fourth = 0b0101;
            break;
        case T1T1:
            first = 0b0101;
            second = 0b0101;
            third = 0b1010;
            fourth = 0b1010;
            break;
        case T1T2:
            first = 0b0101;
            second = 0b1001;
            third = 0b0110;
            fourth = 0b1010;
            break;
        case T1T3:
            first = 0b1001;
            second = 0b0101;
            third = 0b0110;
            fourth = 0b1010;
            break;
        case T2T2:
            first = 0b0101;
            second = 0b1010;
            third = 0b0101;
            fourth = 0b1010;
            break;
        case T2T3:
            first = 0b0101;
            second = 0b1010;
            third = 0b1001;
            fourth = 0b0110;
            break;
        case T3T3:
            first = 0b0101;
            second = 0b1010;
            third = 0b1010;
            fourth = 0b0101;
            break;
        case AN:
            first = 0b0001;
            second = 0b0001;
            third = 0b0001;
            fourth = 0b0001;
            break;
        case AP1:
            first = 0b0011;
            second = 0b0011;
            third = 0b0001;
            fourth = 0b0001;
            break;
        case AP2:
            first = 0b0011;
            second = 0b0001;
            third = 0b0011;
            fourth = 0b0001;
            break;
        case AP3:
            first = 0b0011;
            second = 0b0001;
            third = 0b0001;
            fourth = 0b0011;
            break;
        case AP4:
            first = 0b0001;
            second = 0b0011;
            third = 0b0011;
            fourth = 0b0001;
            break;
        case AP5:
            first = 0b0001;
            second = 0b0011;
            third = 0b0001;
            fourth = 0b0011;
            break;
        case AP6:
            first = 0b0001;
            second = 0b0001;
            third = 0b0011;
            fourth = 0b0011;
            break;
        case AT1:
            first = 0b0011;
            second = 0b0011;
            third = 0b0101;
            fourth = 0b0101;
            break;
        case AT2:
            first = 0b0011;
            second = 0b0101;
            third = 0b0011;
            fourth = 0b0101;
            break;
        case AT3:
            first = 0b0011;
            second = 0b0101;
            third = 0b0101;
            fourth = 0b0011;
            break;
        case AA:
            first = 0b0011;
            second = 0b0011;
            third = 0b0011;
            fourth = 0b0011;
            break;
        default:
            return std::vector<int>{-1, -1, -1, -1};
    }
    return std::vector<int>{first, second, third, fourth};

}

