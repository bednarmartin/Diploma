#pragma once

#include <vector>
#include <unordered_set>

extern std::vector<std::unordered_set<long long int>> perfectMatchings;

void findPerfectMatching(Graph &graph, long long int vertex, std::unordered_set<long long int> &usedEdges,
                         long long int coveredVertices);

void makeCombinationsOfPerfectMatchings(Graph &graph, long long int start,
                                        std::vector<std::unordered_set<long long int>> &usedPerfectMatches);

void determineCombinationsOfPerfectMatchings(Graph &graph);