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
const char* turnAsChar(const Turn& t)
{
    switch (t) {
        case Blue:
            return "B";
        case Red:
            return "R";
        default:
            return "o";
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
const Turn& Board::current() const
{
    return turn;
}

/**
 * Get the number of movements that have been played.
 *
 * @return Number of movements.
 */
const int& Board::countMovements() const
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
const Turn& Board::get(int row, int col) const
{
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

void Board::forEachLine(std::function<void(const char* line)> callback) const
{
    const Graph& graph = getGraph();
    const int& nodes = graph.countNodes();
    const int size = static_cast<int>(sqrt(nodes));
    char line[128] = "";
    Turn piece;

    for (int row = 0; row < size; row++) {
        memset(line, 0, sizeof(line));

        for (int i = 0; i < row * 2; i++) {
            line[i] = ' ';
        }

        int pos = row * 2;

        for (int col = 0; col < size; col++) {
            piece = get(row, col);

            line[pos++] = *turnAsChar(piece);
            
            if (col + 1 < size) {
                line[pos++] = ' ';
                line[pos++] = '-';
                line[pos++] = ' ';
            }
        }

        line[pos++] = '\n';
        line[pos] = '\0';
        callback(line);

        if (row + 1 == size)
            break;

        memset(line, 0, sizeof(line));

        for (int i = 0; i < row * 2 + 1; i++) {
            line[i] = ' ';
        }
        
        pos = row * 2 + 1;

        for (int col = 0; col < size; col++) {
            line[pos++] = '\\';
            
            if (col + 1 < size) {
                line[pos++] = ' ';
                line[pos++] = '/';
                line[pos++] = ' ';
            }
        }

        line[pos++] = '\n';
        line[pos] = '\0';
        
        callback(line);
    }
}

/**
 * Override the << operator in order to facilitate streaming the
 * board over a standard output.
 */
std::ostream& operator<<(std::ostream& os, const Board& board)
{
    board.forEachLine([&os](const char* line) {
        os << line;
    });

    return os;
}