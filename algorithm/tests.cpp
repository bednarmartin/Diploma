#include "graph.cpp"
#include "tests.hpp"
#include <cassert>
#include "binaryfunctions.hpp"

void testGraphReading4g3e() {
    GraphReader graphReader = GraphReader("4g3e.txt");
    Graph graph = graphReader.next();
    assert(graph.adjacencyList[0] == 0b1110);
    assert(graph.adjacencyList[1] == 0b1101);
    assert(graph.adjacencyList[2] == 0b1011);
    assert(graph.adjacencyList[3] == 0b0111);
    assert(graph.numberOfVertices == 4);
    delete[] graph.adjacencyList;
    delete[] graph.edgesList;
    std::cout << "Test: 'graph reading 4g3e' was SUCCESSFUL!" << std::endl;
}

void testGraphReading6g3e() {
    GraphReader graphReader = GraphReader("6g3e.txt");
    int counter = 0;
    while (true) {
        Graph graph = graphReader.next();

        if (!graphReader.hasNext()) {
            break;
        }
        counter++;

        for (int i = 0; i < graph.numberOfVertices; i++) {
            assert(getCountOfOnes(graph.adjacencyList[i]) == 3);
            assert((graph.adjacencyList[i] & (1 << i)) == 0);
        }

        assert(graph.numberOfVertices == 6);

        delete[] graph.adjacencyList;
        delete[] graph.edgesList;
    }
    assert(counter == 2);
    std::cout << "Test: 'graph reading 6g3e' was SUCCESSFUL!" << std::endl;
}

void testMake4Pole() {
    GraphReader graphReader = GraphReader("4g3e.txt");
    while (true) {
        Graph graph = graphReader.next();

        if (!graphReader.hasNext()) {
            break;
        }
        PoleMaker::make4Pole(graph, Edge{0, 1}, Edge{2, 3});
        for (int i = 0; i < 4; i++) {
            assert(graph.adjacencyList[0] == 0b1100);
            assert(graph.adjacencyList[1] == 0b1100);
            assert(graph.adjacencyList[2] == 0b0011);
            assert(graph.adjacencyList[3] == 0b0011);
        }

        PoleMaker::reverseMake4Pole(graph, Edge{0, 1}, Edge{2, 3});
        for (int i = 0; i < 4; i++) {
            assert(graph.adjacencyList[0] == 0b1110);
            assert(graph.adjacencyList[1] == 0b1101);
            assert(graph.adjacencyList[2] == 0b1011);
            assert(graph.adjacencyList[3] == 0b0111);
        }

        PoleMaker::make4Pole(graph, Edge{0, 1}, Edge{0, 2});
        for (int i = 0; i < 4; i++) {
            assert(graph.adjacencyList[0] == 0b1000);
            assert(graph.adjacencyList[1] == 0b1100);
            assert(graph.adjacencyList[2] == 0b1010);
            assert(graph.adjacencyList[3] == 0b0111);
        }

        PoleMaker::reverseMake4Pole(graph, Edge{0, 1}, Edge{0, 2});
        for (int i = 0; i < 4; i++) {
            assert(graph.adjacencyList[0] == 0b1110);
            assert(graph.adjacencyList[1] == 0b1101);
            assert(graph.adjacencyList[2] == 0b1011);
            assert(graph.adjacencyList[3] == 0b0111);
        }
        delete[] graph.adjacencyList;
        delete[] graph.edgesList;
    }
    std::cout << "Test: 'make 4 pole' was SUCCESSFUL!" << std::endl;
}

void testGetCountOfOnes() {
    assert(getCountOfOnes(0b11111) == 5);
    assert(getCountOfOnes(0b11101) == 4);
    assert(getCountOfOnes(0) == 0);
    assert(getCountOfOnes(0b10101110) == 5);
    assert(getCountOfOnes(0b0101010) == 3);
    assert(getCountOfOnes(0b10000000000) == 1);
    std::cout << "Test: 'get count of ones' was SUCCESSFUL!" << std::endl;
}

void testGetOnePositions() {
    std::vector<long long int> positions = getOnePositions(0b10101110);
    assert(positions.size() == getCountOfOnes(0b10101110));
    std::vector<long long int> test{1, 2, 3, 5, 7};
    for (int i = 0; i < 5; i++) {
        assert(test[i] == positions[i]);
    }
    std::vector<long long int> positions2 = getOnePositions(0b100001);
    std::vector<long long int> test2{0, 5};
    for (int i = 0; i < 2; i++) {
        assert(test2[i] == positions2[i]);
    }
    std::cout << "Test: 'get one positions' was SUCCESSFUL!" << std::endl;
}

void testGetOnes() {
    assert(getOnes(5) == 0b11111);
    assert(getOnes(0) == 0);
    assert(getOnes(10) == 0b1111111111);
    assert(getOnes(1) == 0b1);
    assert(getOnes(2) == 0b11);
    assert(getOnes(7) == 0b1111111);
    std::cout << "Test: 'get ones' was SUCCESSFUL!" << std::endl;
}


void runAllTests() {
    testGraphReading4g3e();
    testGraphReading6g3e();
    testMake4Pole();
    testGetOnePositions();
    testGetCountOfOnes();
    testGetOnes();
}