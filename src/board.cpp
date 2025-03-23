#include <iostream>
#include <cmath>
#include "board.hpp"

/**
 * Get a given turn as a single character.
 *
 * @return 'B' for Turn::Blue
 *         'R' for Turn::Red
 *         '·' otherwise
 */
char turnAsChar(const Turn& t)
{
    switch (t) {
        case Blue:
            return 'B';
        case Red:
            return 'R';
        default:
            return 'o';
    }
}

/**
 * Get a given turn as a printable label.
 *
 * @return 'Blue' for Turn::Blue
 *         'Red' for Turn::Red
 *         'Undecided' for Turn::Undecided
 */
std::string turnAsLabel(const Turn& t)
{
    switch (t) {
        case Blue:
            return "Blue";
        case Red:
            return "Red";
        default:
            return "Undecided";
    }
}

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

    movements++;
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
bool Board::exists(int row, int col) const
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
int Board::cell(int row, int col) const
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
Turn Board::current() const
{
    return turn;
}

/**
 * Get the number of movements that have been played.
 *
 * @return Number of movements.
 */
int Board::countMovements() const
{
    return movements;
}

/**
 * Set the given cell with the color of the current player.
 */
void Board::set(int row, int col)
{
    Position position = std::make_pair(row, col);

    if (positions[position] != Turn::Undecided) {
        throw std::invalid_argument("The cell has already been chosen");
    }

    if (opening.first == -1 || opening.second == -1) {
        opening = position;
    }

    positions[position] = turn;
    next();
}

/**
 * Get the color of a given cell.
 *
 * @return The color of the cell.
 */
Turn Board::get(int row, int col) const
{
    if (! exists(row, col)) {
        return Turn::Undecided;
    }

    Position position = std::make_pair(row, col);
    return positions[position];
}

/**
 * Apply the pie rule after the first movement, changing the color
 * of the first piece and passing the turn.
 */
void Board::pieRule()
{
    if (movements != 1) {
        throw std::invalid_argument("The pie rule can only be invoked right after the first move");
    }

    if (turn != Turn::Red) {
        throw std::invalid_argument("The pie rule can only be invoked by the red player after a blue opening");
    }

    positions[opening] = turn;
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
 * Return the board as a string.
 *
 * @return String representation of the board.
 */
std::string Board::toString() const
{
    std::ostringstream os;

    const Graph& graph = getGraph();
    const int nodes = graph.countNodes();
    const int size = static_cast<int>(sqrt(nodes));

    for (int row = 0; row < size; row++) {
        os << std::string(row * 2, ' ');

        for (int col = 0; col < size; col++) {
            Turn piece = get(row, col);

            if (col + 1 < size)
                os << turnAsChar(piece) << " - ";
            else
                os << turnAsChar(piece);
        }

        os << "\n";

        if (row + 1 == size)
            break;

        os << std::string(row * 2 + 1, ' ');

        for (int col = 0; col < size; col++) {
            if (col + 1 < size)
                os << "\\ / ";
            else
                os << "\\";
        }

        os << "\n";
    }

    return os.str();
}

/**
 * Override the << operator in order to facilitate streaming the
 * board over a standard output.
 */
std::ostream& operator<<(std::ostream& os, const Board& board)
{
    os << board.toString();

    return os;
}