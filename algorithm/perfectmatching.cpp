#include "graph.cpp"
#include "binaryfunctions.hpp"
#include "perfectmatching.h"
#include <unordered_set>
#include <unordered_map>
#include "4poles.hpp"

#define PM_A 1
#define PM_P1 2
#define PM_P2 3
#define PM_P3 4
#define PM_P4 5
#define PM_P5 6
#define PM_P6 7
#define PM_N 8


std::vector<std::unordered_set<long long int>> perfectMatchings;

std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeA;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeP1;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeP2;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeP3;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeP4;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeP5;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeP6;
std::vector<std::unordered_set<long long int>> perfectMatchingsOfTypeN;


void findPerfectMatching(Graph &graph, long long int start, std::unordered_set<long long int> &usedEdges,
                         long long int coveredVertices) {
    if (coveredVertices == 0) {
        perfectMatchings.emplace_back(std::unordered_set<long long int>(usedEdges));
        return;
    }
    for (auto i = start; i < graph.numberOfVertices; i++) {
        long long int vertexMask = 1ll << i;
        if ((coveredVertices & vertexMask) == vertexMask) {
            auto adjacency = graph.adjacencyList[i];
            std::vector<long long int> neighbors = getOnePositions(adjacency);
            if (neighbors.size() == 2) {
                neighbors.emplace_back(i);
            }
            for (auto neighbor: neighbors) {
                if (neighbor >= i) {
                    long long int neighborMask = 1ll << neighbor;
                    if ((coveredVertices & neighborMask) == neighborMask) {
                        long long int edge = vertexMask | neighborMask;
                        usedEdges.insert(edge);
                        coveredVertices ^= edge;
                        findPerfectMatching(graph, i + 1, usedEdges, coveredVertices);
                        coveredVertices ^= edge;
                        usedEdges.erase(edge);
                    }
                }
            }
        }
    }
}

void makeCombinationsOfPerfectMatchings(Graph &graph, long long int start,
                                        std::vector<std::unordered_set<long long int>> &usedPerfectMatches) {
    if (usedPerfectMatches.size() == 4) {

        std::unordered_set<long long int> coveredEdges;
        for (auto &perfectMatching: usedPerfectMatches) {
            for (auto edge: perfectMatching) {
                coveredEdges.insert(edge);
            }
        }
        if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
            std::unordered_map<long long int, long long int> results;
            for (auto edge: graph.danglingEdges) {
                results.insert(std::make_pair(edge, 0b0));
            }
            for (auto edge: graph.danglingEdges) {
                long long int matching = 0b1;
                for (auto i = 0; i < 4; i++) {
                    if (usedPerfectMatches[i].contains(edge)) {
                        results[edge] = results[edge] | matching;
                    }
                    matching = matching << 1ll;
                }
            }
            long long int type = determine(results[graph.danglingEdges[0]], results[graph.danglingEdges[1]],
                                           results[graph.danglingEdges[2]],
                                           results[graph.danglingEdges[3]]);

            graph.combinations = graph.combinations | (1ll << type);
            return;
        }
        return;
    }


    for (auto i = start; i < perfectMatchings.size(); i++) {
        std::unordered_set<long long int> perfectMatching = perfectMatchings[i];
        usedPerfectMatches.emplace_back(perfectMatching);
        makeCombinationsOfPerfectMatchings(graph, i + 1, usedPerfectMatches);
        usedPerfectMatches.pop_back();
    }
}

int determineTypeOfPerfectMatching(Graph &graph, std::unordered_set<long long int> &perfectMatching) {
    long long int result = 0;
    long long int matching = 0b1;
    for (auto edge: graph.danglingEdges) {
        if (perfectMatching.contains(edge)) {
            result = result | matching;
        }
        matching = matching << 1ll;
    }
    if (result == 0b0011) {
        return PM_P1;
    } else if (result == 0b0101) {
        return PM_P2;
    } else if (result == 0b1001) {
        return PM_P3;
    } else if (result == 0b0110) {
        return PM_P4;
    } else if (result == 0b1010) {
        return PM_P5;
    } else if (result == 0b1100) {
        return PM_P6;
    } else if (result == 0b1111) {
        return PM_A;
    } else if (result == 0b0000) {
        return PM_N;
    }
    return 0;
}


bool isT1N(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP6,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2N(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP5,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3N(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP3, perfectMatchingOfTypeP4,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1P1(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
        for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
            for (int i = 0; i < perfectMatchingsOfTypeP1.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeP1.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP6, perfectMatchingOfTypeN,
                                                 perfectMatchingsOfTypeP1[i], perfectMatchingsOfTypeP1[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1P2(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
            for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP2,
                                                 perfectMatchingOfTypeP6, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1P3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP3,
                                                 perfectMatchingOfTypeP6, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1P4(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP4,
                                                 perfectMatchingOfTypeP6, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1P5(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
            for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP5,
                                                 perfectMatchingOfTypeP6, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1P6(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
            for (int i = 0; i < perfectMatchingsOfTypeP6.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeP6.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeN,
                                                 perfectMatchingsOfTypeP6[i], perfectMatchingsOfTypeP6[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2P1(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
            for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP2,
                                                 perfectMatchingOfTypeP5, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2P2(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
        for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
            for (int i = 0; i < perfectMatchingsOfTypeP2.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeP2.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP5, perfectMatchingOfTypeN,
                                                 perfectMatchingsOfTypeP2[i], perfectMatchingsOfTypeP2[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2P3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP3,
                                                 perfectMatchingOfTypeP5, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2P4(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP4,
                                                 perfectMatchingOfTypeP5, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2P5(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
            for (int i = 0; i < perfectMatchingsOfTypeP5.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeP5.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeN,
                                                 perfectMatchingsOfTypeP5[i], perfectMatchingsOfTypeP5[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2P6(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
            for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP5,
                                                 perfectMatchingOfTypeP6, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3P1(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP3,
                                                 perfectMatchingOfTypeP4, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3P2(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP3,
                                                 perfectMatchingOfTypeP4, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3P3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
        for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
            for (int i = 0; i < perfectMatchingsOfTypeP3.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeP3.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP4, perfectMatchingOfTypeN,
                                                 perfectMatchingsOfTypeP3[i], perfectMatchingsOfTypeP3[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3P4(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
        for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
            for (int i = 0; i < perfectMatchingsOfTypeP4.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeP4.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP3, perfectMatchingOfTypeN,
                                                 perfectMatchingsOfTypeP4[i], perfectMatchingsOfTypeP4[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3P5(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP3, perfectMatchingOfTypeP4,
                                                 perfectMatchingOfTypeP5, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3P6(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP3, perfectMatchingOfTypeP4,
                                                 perfectMatchingOfTypeP6, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1T1(Graph &graph) {
    for (int i = 0; i < perfectMatchingsOfTypeP1.size(); i++) {
        for (int j = i; j < perfectMatchingsOfTypeP1.size(); j++) {
            for (int k = 0; k < perfectMatchingsOfTypeP6.size(); k++) {
                for (int l = k; l < perfectMatchingsOfTypeP6.size(); l++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingsOfTypeP1[i], perfectMatchingsOfTypeP1[j],
                                                 perfectMatchingsOfTypeP6[k], perfectMatchingsOfTypeP6[l]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }


    }
    return false;
}

bool isT1T2(Graph &graph) {

    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
            for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
                for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP2,
                                                 perfectMatchingOfTypeP5, perfectMatchingOfTypeP6}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT1T3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
                for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP3,
                                                 perfectMatchingOfTypeP4, perfectMatchingOfTypeP6}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT2T2(Graph &graph) {
    for (int i = 0; i < perfectMatchingsOfTypeP2.size(); i++) {
        for (int j = i; j < perfectMatchingsOfTypeP2.size(); j++) {
            for (int k = 0; k < perfectMatchingsOfTypeP5.size(); k++) {
                for (int l = k; l < perfectMatchingsOfTypeP5.size(); l++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingsOfTypeP2[i], perfectMatchingsOfTypeP2[j],
                                                 perfectMatchingsOfTypeP5[k], perfectMatchingsOfTypeP5[l]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }


    }
    return false;
}

bool isT2T3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
                for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP3,
                                                 perfectMatchingOfTypeP4, perfectMatchingOfTypeP5}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isT3T3(Graph &graph) {
    for (int i = 0; i < perfectMatchingsOfTypeP3.size(); i++) {
        for (int j = i; j < perfectMatchingsOfTypeP3.size(); j++) {
            for (int k = 0; k < perfectMatchingsOfTypeP4.size(); k++) {
                for (int l = k; l < perfectMatchingsOfTypeP4.size(); l++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingsOfTypeP3[i], perfectMatchingsOfTypeP3[j],
                                                 perfectMatchingsOfTypeP4[k], perfectMatchingsOfTypeP4[l]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }


    }
    return false;
}

bool isAN(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
            for (int j = i ; j < perfectMatchingsOfTypeN.size(); j++) {
                for (int k = j; k < perfectMatchingsOfTypeN.size(); k++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j],
                                                 perfectMatchingsOfTypeN[k], perfectMatchingOfTypeA}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }


    }
    return false;
}

bool isAP1(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeA, perfectMatchingOfTypeP1,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAP2(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeA, perfectMatchingOfTypeP2,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAP3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeA, perfectMatchingOfTypeP3,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAP4(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeA, perfectMatchingOfTypeP4,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAP5(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeA, perfectMatchingOfTypeP5,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAP6(Graph &graph) {
    for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
        for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
            for (int i = 0; i < perfectMatchingsOfTypeN.size(); i++) {
                for (int j = i; j < perfectMatchingsOfTypeN.size(); j++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeA, perfectMatchingOfTypeP6,
                                                 perfectMatchingsOfTypeN[i], perfectMatchingsOfTypeN[j]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAT1(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP1: perfectMatchingsOfTypeP1) {
        for (const auto &perfectMatchingOfTypeP6: perfectMatchingsOfTypeP6) {
            for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP1, perfectMatchingOfTypeP6,
                                                 perfectMatchingOfTypeA, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAT2(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP2: perfectMatchingsOfTypeP2) {
        for (const auto &perfectMatchingOfTypeP5: perfectMatchingsOfTypeP5) {
            for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP2, perfectMatchingOfTypeP5,
                                                 perfectMatchingOfTypeA, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAT3(Graph &graph) {
    for (const auto &perfectMatchingOfTypeP3: perfectMatchingsOfTypeP3) {
        for (const auto &perfectMatchingOfTypeP4: perfectMatchingsOfTypeP4) {
            for (const auto &perfectMatchingOfTypeA: perfectMatchingsOfTypeA) {
                for (const auto &perfectMatchingOfTypeN: perfectMatchingsOfTypeN) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingOfTypeP3, perfectMatchingOfTypeP4,
                                                 perfectMatchingOfTypeA, perfectMatchingOfTypeN}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isAA(Graph &graph) {
    for (int i = 0; i < perfectMatchingsOfTypeA.size(); i++) {
        for (int j = i; j < perfectMatchingsOfTypeA.size(); j++) {
            for (int k = 0; k < perfectMatchingsOfTypeN.size(); k++) {
                for (int l = k ; l < perfectMatchingsOfTypeN.size(); l++) {
                    std::unordered_set<long long int> coveredEdges;
                    for (auto &perfectMatching: {perfectMatchingsOfTypeA[i], perfectMatchingsOfTypeA[j],
                                                 perfectMatchingsOfTypeN[k], perfectMatchingsOfTypeN[l]}) {
                        for (auto edge: perfectMatching) {
                            coveredEdges.insert(edge);
                        }
                    }
                    if (coveredEdges.size() == ((graph.numberOfVertices * 3) / 2) + 2) {
                        return true;
                    }
                }
            }
        }


    }
    return false;
}

void determineCombinationsOfPerfectMatchings(Graph &graph) {
    for (auto &perfectMatching: perfectMatchings) {
        int type = determineTypeOfPerfectMatching(graph, perfectMatching);
        if (type == PM_A) {
            perfectMatchingsOfTypeA.emplace_back(perfectMatching);
        } else if (type == PM_N) {
            perfectMatchingsOfTypeN.emplace_back(perfectMatching);
        } else if (type == PM_P1) {
            perfectMatchingsOfTypeP1.emplace_back(perfectMatching);
        } else if (type == PM_P2) {
            perfectMatchingsOfTypeP2.emplace_back(perfectMatching);
        } else if (type == PM_P3) {
            perfectMatchingsOfTypeP3.emplace_back(perfectMatching);
        } else if (type == PM_P4) {
            perfectMatchingsOfTypeP4.emplace_back(perfectMatching);
        } else if (type == PM_P5) {
            perfectMatchingsOfTypeP5.emplace_back(perfectMatching);
        } else if (type == PM_P6) {
            perfectMatchingsOfTypeP6.emplace_back(perfectMatching);
        } else {
            std::cout << "ERROR 1" << std::endl;
        }
    }

    if (isT1N(graph)) {
        graph.combinations = graph.combinations | (1ll << T1N);
    }
    if (isT2N(graph)) {
        graph.combinations = graph.combinations | (1ll << T2N);
    }
    if (isT3N(graph)) {
        graph.combinations = graph.combinations | (1ll << T3N);
    }

    if (isT1P1(graph)) {
        graph.combinations = graph.combinations | (1ll << T1P1);
    }

    if (isT1P2(graph)) {
        graph.combinations = graph.combinations | (1ll << T1P2);
    }

    if (isT1P3(graph)) {
        graph.combinations = graph.combinations | (1ll << T1P3);
    }
    if (isT1P4(graph)) {
        graph.combinations = graph.combinations | (1ll << T1P4);
    }
    if (isT1P5(graph)) {
        graph.combinations = graph.combinations | (1ll << T1P5);
    }
    if (isT1P6(graph)) {
        graph.combinations = graph.combinations | (1ll << T1P6);
    }

    if (isT2P1(graph)) {
        graph.combinations = graph.combinations | (1ll << T2P1);
    }

    if (isT2P2(graph)) {
        graph.combinations = graph.combinations | (1ll << T2P2);
    }

    if (isT2P3(graph)) {
        graph.combinations = graph.combinations | (1ll << T2P3);
    }
    if (isT2P4(graph)) {
        graph.combinations = graph.combinations | (1ll << T2P4);
    }
    if (isT2P5(graph)) {
        graph.combinations = graph.combinations | (1ll << T2P5);
    }
    if (isT2P6(graph)) {
        graph.combinations = graph.combinations | (1ll << T2P6);
    }
    if (isT3P1(graph)) {
        graph.combinations = graph.combinations | (1ll << T3P1);
    }

    if (isT3P2(graph)) {
        graph.combinations = graph.combinations | (1ll << T3P2);
    }

    if (isT3P3(graph)) {
        graph.combinations = graph.combinations | (1ll << T3P3);
    }
    if (isT3P4(graph)) {
        graph.combinations = graph.combinations | (1ll << T3P4);
    }
    if (isT3P5(graph)) {
        graph.combinations = graph.combinations | (1ll << T3P5);
    }
    if (isT3P6(graph)) {
        graph.combinations = graph.combinations | (1ll << T3P6);
    }
    if (isAP1(graph)) {
        graph.combinations = graph.combinations | (1ll << AP1);
    }
    if (isAP2(graph)) {
        graph.combinations = graph.combinations | (1ll << AP2);
    }
    if (isAP3(graph)) {
        graph.combinations = graph.combinations | (1ll << AP3);
    }
    if (isAP4(graph)) {
        graph.combinations = graph.combinations | (1ll << AP4);
    }
    if (isAP5(graph)) {
        graph.combinations = graph.combinations | (1ll << AP5);
    }
    if (isAP6(graph)) {
        graph.combinations = graph.combinations | (1ll << AP6);
    }
    if (isAT1(graph)) {
        graph.combinations = graph.combinations | (1ll << AT1);
    }
    if (isAT2(graph)) {
        graph.combinations = graph.combinations | (1ll << AT2);
    }
    if (isAT3(graph)) {
        graph.combinations = graph.combinations | (1ll << AT3);
    }
    if (isAA(graph)) {
        graph.combinations = graph.combinations | (1ll << AA);
    }
    if (isT1T1(graph)) {
        graph.combinations = graph.combinations | (1ll << T1T1);
    }
    if (isT1T2(graph)) {
        graph.combinations = graph.combinations | (1ll << T1T2);
    }
    if (isT1T3(graph)) {
        graph.combinations = graph.combinations | (1ll << T1T3);
    }
    if (isT2T2(graph)) {
        graph.combinations = graph.combinations | (1ll << T2T2);
    }
    if (isT2T3(graph)) {
        graph.combinations = graph.combinations | (1ll << T2T3);
    }
    if (isT3T3(graph)) {
        graph.combinations = graph.combinations | (1ll << T3T3);
    }
    if (isAN(graph)) {
        graph.combinations = graph.combinations | (1ll << AN);
    }

    perfectMatchingsOfTypeN.clear();
    perfectMatchingsOfTypeP6.clear();
    perfectMatchingsOfTypeP1.clear();
    perfectMatchingsOfTypeP2.clear();
    perfectMatchingsOfTypeP4.clear();
    perfectMatchingsOfTypeP3.clear();
    perfectMatchingsOfTypeP5.clear();
    perfectMatchingsOfTypeA.clear();


}
