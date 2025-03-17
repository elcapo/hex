#include <cstddef>
#include <cstdlib>
#include <iostream>
#include "graph.hpp"
// #include "board.hpp"

static int allocations = 0;

void* operator new(size_t size)
{
    allocations++;
    return malloc(size);
}

int main()
{
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);
    graph.connect(1, 2);
    graph.connect(1, 3);
    graph.connect(1, 4);
    graph.connect(3, 1);

    std::cout << "Nodes: " << graph.countNodes() << std::endl;
    std::cout << "Edges: " << graph.countEdges() << std::endl;

    graph.forEachEdge([](const Edge& edge) {
        std::cout << "> Edge from: " << edge.from << " to: " << edge.to << std::endl;
    });

    std::cout << "Allocations: " << allocations << std::endl;
}