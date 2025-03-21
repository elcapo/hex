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
    Board board(7);
    const Graph& graph = board.getGraph();

    board.set(1, 1);
    board.pieRule();
    board.set(2, 1);
    board.set(2, 2);

    std::cout << "Nodes: " << graph.countNodes() << " / ";
    std::cout << "Edges: " << graph.countEdges() << " / ";
    std::cout << "Turn: " << turnAsChar(board.current()) << " / ";
    std::cout << "Movements: " << board.countMovements();
    std::cout << std::endl << std::endl;

    std::cout << board << std::endl;

    std::cout << "Allocations: " << allocations << std::endl;
}