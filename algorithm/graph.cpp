#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <unordered_map>


struct Edge {
    long long int vertex1;
    long long int vertex2;

    friend std::ostream &operator<<(std::ostream &os, const Edge &edge) {
        os << "[" << edge.vertex1 << " " << edge.vertex2 << "]";
        return os;
    }

    bool operator==(const Edge &other) const {
        return (vertex1 == other.vertex1 || vertex1 == other.vertex2) &&
               (vertex2 == other.vertex1 || vertex2 == other.vertex2);
    }

};
namespace std {
    template<>
    struct hash<Edge> {
        size_t operator()(const Edge &v) const {
            return (v.vertex1 * v.vertex2) + (v.vertex1 + v.vertex2);
        }
    };
}


struct Graph {
    long long int numberOfVertices;
    long long int *adjacencyList;
    Edge *edgesList;
    std::vector<long long int> danglingEdges;
    std::unordered_map<Edge, int> edgeMapping;
    Edge **edges;
    long long int combinations = 0ll;
    bool hasBridge = false;

    explicit Graph(long long int numberOfVertices) {
        this->numberOfVertices = numberOfVertices;
        adjacencyList = new long long int[numberOfVertices];
        edgesList = new Edge[(3 * numberOfVertices) / 2];
        edges = new Edge *[numberOfVertices];
        for (int i = 0; i < numberOfVertices; i++) {
            edges[i] = new Edge[3];
        }

    }

    void determineBridge() {
        auto V = numberOfVertices;
        bool *visited = new bool[V];
        auto *disc = new unsigned long long int[V];
        auto *low = new unsigned long long int[V];
        auto *parent = new unsigned long long int[V];
        for (auto i = 0; i < V; i++) {
            parent[i] = -1;
            visited[i] = false;
        }
        for (auto i = 0; i < V; i++)
            if (!visited[i])
                bridgeRecursion(i, visited, disc, low, parent, 0);

        delete[] visited;
        delete[] disc;
        delete[] low;
        delete[] parent;
    }


private:
    void bridgeRecursion(unsigned long long int vertex, bool visited[], unsigned long long int disc[],
                         unsigned long long int low[], unsigned long long int parent[], unsigned long long int time) {
        if (hasBridge) {
            return;
        }
        visited[vertex] = true;
        disc[vertex] = low[vertex] = ++time;
        long long int neighborsEncoded = adjacencyList[vertex];
        long long int counter = 0ll;
        long long int binaryPosition = 1ll;
        std::vector<long long int> neighbors;
        while (neighbors.size() < 3) {
            if ((neighborsEncoded & binaryPosition) != 0) {
                neighbors.emplace_back(counter);
            }
            counter++;
            binaryPosition <<= 1ll;
        }
        for (long long int neighbor: neighbors) {
            if (!visited[neighbor]) {
                parent[neighbor] = vertex;
                bridgeRecursion(neighbor, visited, disc, low, parent, time);
                low[vertex] = std::min(low[vertex], low[neighbor]);

                //[vertex,neighbor] is a bridge
                if (low[neighbor] > disc[vertex]) {
                    hasBridge = true;
                    return;
                }

            } else if (neighbor != parent[vertex])
                low[vertex] = std::min(low[vertex], disc[neighbor]);
        }
    }

};


class GraphReader {
    std::ifstream inData;
    long long int numberOfVertices{};
    bool hasNextGraph = true;

public:
    explicit GraphReader(const char *path) {
        inData.open(path);
        if (!inData) {
            std::cerr << "Error: File could not be opened" << std::endl;
        }
        inData >> numberOfVertices;
    }

    Graph next() {
        auto graph = Graph(numberOfVertices);
        long long int edgesCounter = 0ll;
        for (auto i = 0; i < numberOfVertices && hasNextGraph; i++) {
            long long int adjacency = 0ll;
            for (auto j = 0; j < 3; j++) {
                long long int neighbor;
                if (inData >> neighbor) {
                    adjacency = adjacency | (1ll << neighbor);
                    if (i < neighbor) {
                        graph.edgesList[edgesCounter] = Edge{i, neighbor};
                        graph.edges[i][j] = Edge{i, neighbor};
                        edgesCounter++;
                    } else {
                        graph.edges[i][j] = Edge{neighbor, i};
                    }

                } else {
                    hasNextGraph = false;
                    delete[] graph.adjacencyList;
                    delete[] graph.edgesList;
                    return graph;
                }
            }
            graph.adjacencyList[i] = adjacency;
        }
        return graph;
    }

    bool hasNext() const {
        return hasNextGraph;
    }

};

class PoleMaker {
public:
    static void reverseMake4Pole(Graph &graph, Edge first, Edge second) {
        toggleEdges(graph, first, second);
        graph.danglingEdges.clear();
        graph.combinations = 0ll;
    }

    static void make4Pole(Graph &graph, Edge edge1, Edge edge2) {
        toggleEdges(graph, edge1, edge2);
        graph.danglingEdges.emplace_back(1ll << edge1.vertex1);
        graph.danglingEdges.emplace_back(1ll << edge1.vertex2);
        graph.danglingEdges.emplace_back(1ll << edge2.vertex1);
        graph.danglingEdges.emplace_back(1ll << edge2.vertex2);
    }

    static void makeObjectOriented4Pole(Graph &graph, Edge edge1, Edge edge2){


    }


private:
    static void toggleEdges(Graph &graph, Edge edge1, Edge edge2) {
        graph.adjacencyList[edge1.vertex1] ^= 1ll << edge1.vertex2;
        graph.adjacencyList[edge1.vertex2] ^= 1ll << edge1.vertex1;
        graph.adjacencyList[edge2.vertex1] ^= 1ll << edge2.vertex2;
        graph.adjacencyList[edge2.vertex2] ^= 1ll << edge2.vertex1;
    }
};