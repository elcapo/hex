#include <cstddef>
#include <cstdlib>
#include <iostream>
#include "graph.hpp"
#include "board.hpp"

static int allocations = 0;

void* operator new(size_t size)
{
    allocations++;
    return malloc(size);
}

int main()
{
    Board board(3);
    const Graph& graph = board.getGraph();

    std::cout << "Nodes: " << graph.countNodes() << " / Edges: " << graph.countEdges() << std::endl << std::endl;
    std::cout << graph << std::endl;
    std::cout << "Allocations: " << allocations << std::endl;
}