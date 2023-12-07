#include "graph.cpp"
#include "SATperfectmatching.h"
#include "cryptominisat5/cryptominisat.h"
#include "4poles.hpp"

using namespace CMSat;

long long int findPerfectMatchingSATWithDanglingEdges(Graph &graph, Edge edge1, Edge edge2, int numOfThreads) {
    long long int representation = 0ll;
    std::unordered_map<Edge, int> edges;
    std::unordered_map<int, Edge> reverseEdges;
    int counter = 0;
    for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
        Edge *outgoingEdges = graph.edges[vertex];
        for (int i = 0; i < 3; i++) {
            Edge edge = outgoingEdges[i];
            if (!edges.contains(edge) && edge != edge1 && edge != edge2) {
                edges[edge] = counter;
                reverseEdges[counter] = edge;
                counter++;
            }
        }

    }
    Edge danglingEdge1 = Edge{edge1.vertex1, -1};
    Edge danglingEdge2 = Edge{edge1.vertex2, -1};
    Edge danglingEdge3 = Edge{edge2.vertex1, -1};
    Edge danglingEdge4 = Edge{edge2.vertex2, -1};
    edges[danglingEdge1] = counter;
    edges[danglingEdge2] = counter + 1;
    edges[danglingEdge3] = counter + 2;
    edges[danglingEdge4] = counter + 3;
    reverseEdges[counter] = danglingEdge1;
    reverseEdges[counter + 1] = danglingEdge2;
    reverseEdges[counter + 2] = danglingEdge3;
    reverseEdges[counter + 3] = danglingEdge4;

    int offset = (3 * graph.numberOfVertices) / 2 + 2;

    //SAT
    for (int type = 0; type < 38; type++) {
        SATSolver solver;
        std::vector<Lit> clause;
        solver.set_num_threads(numOfThreads);

        solver.new_vars(6 * graph.numberOfVertices + 8);


        // At least one outgoing edge for each vertex

        for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
            for (int j = 0; j < 4; j++) {
                Edge *outgoingEdges = graph.edges[vertex];
                for (int i = 0; i < 3; i++) {
                    Edge edge = outgoingEdges[i];
                    if (edge != edge1 && edge != edge2) {
                        int number = edges[edge] + j * offset;
                        clause.emplace_back(number, false);
                    }
                }
                Edge danglingEdge = Edge{vertex, -1};
                if (edges.contains(danglingEdge)) {
                    clause.emplace_back(edges[danglingEdge] + j * offset, false);
                }
                solver.add_clause(clause);
                clause.clear();
            }
        }

        // At most one outgoing edge for each vertex

        for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
            for (int j = 0; j < 4; j++) {
                Edge *outgoingEdges = graph.edges[vertex];
                for (int i = 0; i < 3; i++) {
                    Edge edge = outgoingEdges[i];
                    for (int k = i + 1; k < 3; k++) {
                        Edge edge_ = outgoingEdges[k];
                        int number1, number2;
                        if (edge != edge1 && edge != edge2 && edge_ != edge1 && edge_ != edge2) {
                            number1 = edges[edge] + j * offset;
                            number2 = edges[edge_] + j * offset;
                        } else if (edge != edge1 && edge != edge2) {
                            number1 = edges[edge] + j * offset;
                            assert(edges.contains(Edge{vertex, -1}));
                            number2 = edges[Edge{vertex, -1}] + j * offset;
                        } else {
                            assert(edges.contains(Edge{vertex, -1}));
                            number1 = edges[Edge{vertex, -1}] + j * offset;
                            number2 = edges[edge_] + j * offset;
                        }
                        clause.emplace_back(number1, true);
                        clause.emplace_back(number2, true);
                        solver.add_clause(clause);
                        clause.clear();
                    }

                }
            }
        }

        // Each edge must be covered
        for (int number = 0; number < offset; number++) {
            for (int i = 0; i < 4; i++) {
                clause.emplace_back(number + i * offset, false);
            }
            assert(clause.size() == 4);
            solver.add_clause(clause);
            clause.clear();
        }

        // one of the 38 types
        std::vector<int> example = getExample(type);
        for (int i = 0; i < 4; i++) {
            clause.emplace_back(edges[danglingEdge1] + i * offset, !((example[0]) & (1 << i)));
            solver.add_clause(clause);
            clause.clear();

            clause.emplace_back(edges[danglingEdge2] + i * offset, !((example[1]) & (1 << i)));
            solver.add_clause(clause);
            clause.clear();

            clause.emplace_back(edges[danglingEdge3] + i * offset, !((example[2]) & (1 << i)));
            solver.add_clause(clause);
            clause.clear();

            clause.emplace_back(edges[danglingEdge4] + i * offset, !((example[3]) & (1 << i)));
            solver.add_clause(clause);
            clause.clear();
        }

        lbool ret = solver.solve();
        if (ret == l_True) {
            representation = representation | (1ll << type);
        }

    }
    return representation;


}

long long int findPerfectMatchingSAT(Graph &graph) {
    std::unordered_map<Edge, int> edges;
    std::unordered_map<int, Edge> reverseEdges;
    int counter = 0;
    int countOfClauses = 0;
    for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
        Edge *outgoingEdges = graph.edges[vertex];
        for (int i = 0; i < 3; i++) {
            Edge edge = outgoingEdges[i];
            if (!edges.contains(edge)) {
                edges[edge] = counter;
                reverseEdges[counter] = edge;
                counter++;
            }
        }
    }

    long long int offset = (3 * graph.numberOfVertices) / 2;

    //SAT
    SATSolver solver;
    std::vector<Lit> clause;
    solver.set_num_threads(6);

    solver.new_vars(6 * graph.numberOfVertices);

    // At least one outgoing edge for each vertex

    for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
        for (int j = 0; j < 4; j++) {
            Edge *outgoingEdges = graph.edges[vertex];
            for (int i = 0; i < 3; i++) {
                Edge edge = outgoingEdges[i];
                long long int number = edges[edge] + j * offset;
                clause.emplace_back(number, false);

            }
            assert(clause.size() == 3);
            solver.add_clause(clause);
            countOfClauses++;
            clause.clear();
        }
    }

    // At most one outgoing edge for each vertex

    for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
        for (int j = 0; j < 4; j++) {
            Edge *outgoingEdges = graph.edges[vertex];
            for (int i = 0; i < 3; i++) {
                Edge edge = outgoingEdges[i];
                for (int k = i + 1; k < 3; k++) {
                    Edge edge_ = outgoingEdges[k];
                    long long int number1 = edges[edge] + j * offset;
                    long long int number2 = edges[edge_] + j * offset;
                    clause.emplace_back(number1, true);
                    clause.emplace_back(number2, true);
                    solver.add_clause(clause);
                    countOfClauses++;
                    clause.clear();
                }

            }
        }
    }

    // Each edge must be covered
    for (int number = 0; number < offset; number++) {
        for (int i = 0; i < 4; i++) {
            clause.emplace_back(number + i * offset, false);
        }
        assert(clause.size() == 4);
        solver.add_clause(clause);
        countOfClauses++;
        clause.clear();
    }


    lbool ret = solver.solve();
    if (ret == l_True) {
        std::cout << "SATISFIABLE" << std::endl;
        for (int j = 0; j < 4; j++) {
            std::cout << j + 1 << ". PERFECT MATCHING COVER:" << std::endl;
            for (int vertex = 0; vertex < graph.numberOfVertices; vertex++) {
                std::cout << "VERTEX = " << vertex << std::endl;
                Edge *outgoingEdges = graph.edges[vertex];
                for (int i = 0; i < 3; i++) {
                    Edge edge = outgoingEdges[i];
                    std::cout << "[ " << edge.vertex1 << " " << edge.vertex2 << "] - ";
                    long long int number = edges[edge] + j * offset;
                    std::cout << solver.get_model()[number] << std::endl;
                }
            }
        }
        return 1ll;
    } else {
        std::cout << "UNSATISFIABLE" << std::endl;
        return 0ll;
    }


}


