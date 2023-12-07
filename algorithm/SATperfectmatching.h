#pragma once

#include <unordered_set>

long long int findPerfectMatchingSATWithDanglingEdges(Graph &graph, Edge edge1, Edge edge2, int numOfThreads);

long long int findPerfectMatchingSAT(Graph &graph);