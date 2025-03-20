#include <iostream>
#include <cmath>
#include "board.hpp"

/**
 * Passes the turn to the next player
 */
void Board::next()
{
    if (turn == Turn::Blue) {
        turn = Turn::Red;
    } else {
        turn = Turn::Blue;
    }
}

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
 *         Also can return -1, if the cell does not exist.
 */
int Board::cell(int row, int col)
{
    if (! exists(row, col)) {
        return -1;
    }

    return row * size + col;
}

/**
 * Get the color of the current turn.
 *
 * @return The color of the current turn.
 */
Turn Board::current()
{
    return turn;
}

/**
 * Set the given cell with the color of the current player.
 */
void Board::set(int row, int col)
{
    if (pieces.count({row, col}) > 0 && pieces[{row, col}] != Turn::Undecided) {
        throw std::invalid_argument("The cell has already been chosen");
    }

    if (pieces.size() == 0) {
        opening = {row, col};
    }

    pieces[{row, col}] = turn;
    next();
}

/**
 * Get the color of a given cell.
 *
 * @return The color of the cell.
 */
Turn Board::get(int row, int col)
{
    if (! exists(row, col)) {
        return Turn::Undecided;
    }

    return pieces[{row, col}];
}

/**
 * Apply the pie rule after the first movement, changing the color
 * of the first piece and passing the turn.
 */
void Board::pieRule()
{
    if (pieces.size() != 1) {
        throw std::invalid_argument("The pie rule can only be invoked right after the first move");
    }

    if (turn != Turn::Red) {
        throw std::invalid_argument("The pie rule can only be invoked by the red player after a blue opening");
    }

    pieces[opening] = turn;
    next();
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
 * board over a standard output.
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