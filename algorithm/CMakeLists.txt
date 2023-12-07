cmake_minimum_required(VERSION 3.23)
project(PerfectMatchingIndex)

set(CMAKE_CXX_STANDARD 23)

add_executable(PerfectMatchingIndex main.cpp 4poles.cpp 4poles.hpp binaryfunctions.cpp binaryfunctions.hpp perfectmatching.cpp graph.cpp tests.cpp tests.hpp perfectmatching.h kempeswitches.cpp kempeswitches.h permutations.cpp permutations.h SATperfectmatching.cpp SATperfectmatching.h disjuction.cpp disjuction.h
        SATperfectmatching.cpp
        VF3.h
        VF3.cpp)
