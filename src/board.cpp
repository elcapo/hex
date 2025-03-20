#include <iostream>
#include <cmath>
#include "board.hpp"

/**
 * Checks if a cell exists by its row and column numbers.
 *
 * Both the row and the column number are expected to be
 * in the range from 0 to size-1.
 *
 * @param row The row number.
 * @param col The column number.
 *
 * @return Whether the cell exists or not.
 */
bool Board::exists(int row, int col)
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

/**
 * Obtain the number of a cell counting from its top left
 * cell, in a first left then down order.
 *
 * @param row The row number.
 * @param col The column number.
 *
 * @return A number for each cell in a range from 0 to size-1.
 */
int Board::cell(int row, int col)
{
    if (row < 0 || row > size) {
        throw std::invalid_argument("Can't determine a cell number due to an invalid row number");
    }

    if (col < 0 || col > size) {
        throw std::invalid_argument("Can't determine a cell number due to an invalid column number");
    }

    return row * size + col;
}

/**
 * Get a read only version of the graph.
 *
 * @return Read only pointer to the graph.
 */
const Graph& Board::getGraph() const {
    return graph;
}

/**
 * Override the << operator in order to facilitate streaming the
 * board over a standard output
 */
std::ostream& operator<<(std::ostream& os, const Board& board)
{
    const Graph& graph = board.getGraph();
    const int nodes = graph.countNodes();
    const int size = static_cast<int>(sqrt(nodes));

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            os << ".";
        }

        os << std::endl;
    }

    return os;
}