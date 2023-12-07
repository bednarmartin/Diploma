#include <iostream>
#include <ctime>
#include "tests.hpp"
#include "graph.cpp"
#include "unordered_set"
#include "perfectmatching.h"
#include "binaryfunctions.hpp"
#include "kempeswitches.h"
#include "SATperfectmatching.h"
#include "disjuction.h"
#include <climits>
#include "VF3.h"

void doKempe();

void doRepresentantTest(char *const *argv);

void doDisjunction(char *const *argv);

void doTest(int argc, char *const *argv);

void doConvert(char *const *argv);

void poleTestWithoutSAT(Edge &edge1, Edge &edge2, Graph &graph);

long long int poleTestWithSAT(Edge &edge1, Edge &edge2, Graph &graph);

void doTestPole(int argc, char **argv);

void doTestIsUnsatisfiable(int argc, char **argv);

void processGraph(Graph &graph, std::ofstream &outdata, bool SAT, bool noDangling) {
    if (noDangling) {
        findPerfectMatchingSAT(graph);
        return;
    }
    for (int i = 0; i < (3 * graph.numberOfVertices) / 2; i++) {
        Edge edge1 = graph.edgesList[i];
        for (int j = i + 1; j < (3 * graph.numberOfVertices) / 2; j++) {
            Edge edge2 = graph.edgesList[j];
            if (std::unordered_set{edge1.vertex1, edge1.vertex2, edge2.vertex1, edge2.vertex2}.size() == 4) {
                if (SAT) {
                    std::cout << getRepresentant(poleTestWithSAT(edge1, edge2, graph)) << " - [" << edge1.vertex1 << " "
                              << edge1.vertex2 << "],[" << edge2.vertex1 << " " << edge2.vertex2 << "]" << std::endl;
                    outdata << getRepresentant(poleTestWithSAT(edge1, edge2, graph)) << " - [" << edge1.vertex1 << " "
                            << edge1.vertex2 << "],[" << edge2.vertex1 << " " << edge2.vertex2 << "]" << std::endl;
                } else {
                    poleTestWithoutSAT(edge1, edge2, graph);

                }
            }
        }
    }
}

long long int poleTestWithSAT(Edge &edge1, Edge &edge2, Graph &graph) {
    return findPerfectMatchingSATWithDanglingEdges(graph, edge1, edge2, 2);
}

void poleTestWithoutSAT(Edge &edge1, Edge &edge2, Graph &graph) {
    perfectMatchings.clear();
    PoleMaker::make4Pole(graph, edge1, edge2);
    std::unordered_set<long long int> usedEdges;
    findPerfectMatching(graph, 0ll, usedEdges, getOnes(graph.numberOfVertices));
    std::vector<std::unordered_set<long long int>> usedPerfectMatches;
    determineCombinationsOfPerfectMatchings(graph);
    PoleMaker::reverseMake4Pole(graph, edge1, edge2);
}

bool isGood(long long int representation) {
    initializePermutations();
    initializeMasks();
    return isRepresentant(representation) && isKempeClosed(representation);
}

void doKempe() {
    std::ofstream kempeOutdata("KempeOutput.txt");
    determineAllPossibleRepresentants(kempeOutdata);
    kempeOutdata.close();
}

void doRepresentantTest(char *const *argv) {
    if (isGood(std::stoll(argv[2]))) {
        std::cout << "IS GOOD" << std::endl;
    } else {
        std::cout << "IS BAD" << std::endl;
        if (isRepresentant(std::stoll(argv[2]))) {
            std::cout << "NUMBER IS REPRESENTANT" << std::endl;
        } else {
            std::cout << "NUMBER IS NOT REPRESENTANT" << std::endl;
            std::cout << "REPRESENTANT IS " << getRepresentant((std::stoll(argv[2]))) << std::endl;
        }
    }
    std::cout << convertRepresentation(std::stoll(argv[2])) << std::endl;
}

void doConvert(char *const *argv) {
    long long int representation = std::stoll(argv[2]);
    std::cout << convertRepresentation(representation) << std::endl;
}

void doTest(int argc, char *const *argv) {
    bool SAT = true;
    bool noDangling = false;

    GraphReader graphReader = GraphReader(argv[2]);
    long long int counter = 1;
    std::ofstream outdata(argv[3]);
    int threshold = 0;
    int end = INT_MAX;

    if (argc >= 6) {
        threshold = std::stoi(argv[4]);
        end = std::stoi(argv[5]);
    }

    while (true) {
        Graph graph = graphReader.next();
        if (!graphReader.hasNext() || counter >= end) {
            break;
        }
        std::cout << "GRAPH " << counter << std::endl;
        graph.determineBridge();
        if (graph.hasBridge || counter < threshold) {
            std::cout << "HAS BRIDGE" << std::endl;
        } else {
            outdata << "GRAPH " << counter << " - " << argv[2] << std::endl;
            processGraph(graph, outdata, SAT, noDangling);
            outdata << std::endl;
        }

        delete[] graph.adjacencyList;
        delete[] graph.edgesList;
        counter++;
    }
    outdata.close();
}

void doDisjunction(char *const *argv) {
    std::ofstream outdata(argv[3]);
    std::vector<long long int> representants = getRepresentants(argv[2]);
    for (int i = 0; i < representants.size(); i++) {
        long long int representant1 = representants[i];
        for (int j = i + 1; j < representants.size(); j++) {
            bool was = false;
            long long int representant2 = representants[j];
            for (int p1 = 0; p1 < 24; p1++) {
                if (was) {
                    break;
                }
                long long int newRepresentant1 = getPermutation(representant1, p1);
                for (int p2 = 0; p2 < 24; p2++) {
                    if (was) {
                        break;
                    }
                    long long int newRepresentant2 = getPermutation(representant2, p2);
                    if ((newRepresentant1 & newRepresentant2) == 0) {
                        outdata << representant1 << ": permutation = " << p1 << " making it: " << newRepresentant1
                                << std::endl;
                        std::vector<int> numbers = {1, 2, 3, 4};;
                        getPermutation(numbers, 0, p1, outdata);
                        counterToZero();
                        outdata << representant2 << ": permutation = " << p2 << " making it: " << newRepresentant2
                                << std::endl;
                        std::vector<int> numbers2 = {1, 2, 3, 4};;
                        getPermutation(numbers2, 0, p2, outdata);
                        counterToZero();
                        was = true;
                    }
                }
            }
        }
    }
    outdata.close();
}

void doAutomaticAllDisjunctionPermutations(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    GraphReader graphReader1 = GraphReader(argv[2]);
    GraphReader graphReader2 = GraphReader(argv[3]);

    Graph graph1 = graphReader1.next();
    Graph graph2 = graphReader2.next();

    Edge edge1First = Edge(std::stoi(argv[4]), std::stoi(argv[5]));
    Edge edge1Second = Edge(std::stoi(argv[6]), std::stoi(argv[7]));

    Edge edge2First = Edge(std::stoi(argv[8]), std::stoi(argv[9]));
    Edge edge2Second = Edge(std::stoi(argv[10]), std::stoi(argv[11]));

    long long int representant1 = poleTestWithSAT(edge1First, edge1Second, graph1);
    long long int representant2 = poleTestWithSAT(edge2First, edge2Second, graph2);


    for (int p1 = 0; p1 < 24; p1++) {
        long long int newRepresentant1 = getPermutation(representant1, p1);
        for (int p2 = 0; p2 < 24; p2++) {
            long long int newRepresentant2 = getPermutation(representant2, p2);
            if ((newRepresentant1 & newRepresentant2) == 0) {

            }
        }

    }
}


void doTestIsUnsatisfiable(int argc, char **argv) {
    GraphReader graphReader = GraphReader(argv[2]);
    Graph graph = graphReader.next();
    findPerfectMatchingSAT(graph);

}

void doTestPole(int argc, char **argv) {
    GraphReader graphReader = GraphReader(argv[2]);
    Graph graph = graphReader.next();
    Edge first = Edge(22, 31);
    Edge second = Edge(0, 25);
    std::ofstream outdata(argv[3]);
    poleTestWithSAT(first, second, graph);
}

int main([[maybe_unused]] int argc, char **argv) {
    //runAllTests();
    initializePermutations();

    if (argv[1] == std::string("kempe")) {
        doKempe();
    } else if (argv[1] == std::string("representant_test")) {
        doRepresentantTest(argv);
    } else if (argv[1] == std::string("disjunction")) {
        doDisjunction(argv);
    } else if (argv[1] == std::string("convert")) {
        doConvert(argv);
    } else if (argv[1] == std::string("test")) {
        doTest(argc, argv);
    } else if (argv[1] == std::string("test_pole")) {
        doTestPole(argc, argv);
    } else if (argv[1] == std::string("automatic_all_disjunction_permutations")) {
        doAutomaticAllDisjunctionPermutations(argc, argv);
    } else if (argv[1] == std::string("test_is_unsatisfiable")) {
        doTestIsUnsatisfiable(argc, argv);
    } else if (argv[1] == std::string("test_isomorphism")) {
        char *arguments[4] = {argv[0], argv[2], argv[3], NULL};
        int result = isIsomorphic(4, arguments);
        std::cout << result << std::endl;
    }
    deletePermutation();
    return 0;
}







