#ifndef BOARD_H
#define BOARD_H

#include "graph.hpp"

/**
 * The `Board` class models a board for the Hex game by implementing
 * it as a graph.
 */
class Board
{
private:
    // The number of rows and also the number of columns
    int size;

    // The graph that represents cells and their connections
    Graph graph;
public:
    Board(int size) : size(size), graph(size*size, size*size*6)
    {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                // Connect every cell with its right neighbour
                if (exists(row, col + 1)) {
                    graph.connect(cell(row, col), cell(row, col + 1));
                }

                // Connect every cell with its bottom neighbour
                if (exists(row + 1, col)) {
                    graph.connect(cell(row, col), cell(row + 1, col));
                }

                // Connect every cell with its bottom-right neighbour
                if (exists(row + 1, col + 1)) {
                    graph.connect(cell(row, col), cell(row + 1, col + 1));
                }
            }
        }
    };

    /**
     * Checks if a cell exists by its row and column numbers.
     *
     * Both the row and the column number are expected to be
     * in the range from 0 to the "size - 1" of the board.
     *
     * @param row The row number.
     * @param col The column number.
     *
     * @return Whether the cell exists or not.
     */
    bool exists(int row, int col);

    /**
     * Obtain the number of a cell counting from its top left
     * cell, in a first left then down order.
     *
     * @param row The row number.
     * @param col The column number.
     *
     * @return A number for each cell in a range from 0 to size-1.
     */
    int cell(int row, int col);

    /**
     * Get a read only version of the graph.
     *
     * @return Read only pointer to the graph.
     */
    const Graph& getGraph() const;

    /**
     * Override the << operator in order to facilitate streaming the
     * board over a standard output
     */
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

#endif // BOARD_H