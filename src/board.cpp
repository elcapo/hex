#include <iostream>
#include <cmath>
#include "board.hpp"

/**
 * Get a given turn as a single character.
 *
 * @return 'B' for Turn::Blue
 *         'R' for Turn::Red
 *         'o' otherwise
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
            return "Blue plays";
        case Red:
            return "Red plays";
        default:
            return "Game finished";
    }
}

/**
 * Passes the turn to the next player
 */
void Board::next()
{
    if (turn == Turn::Blue) {
        turn = Turn::Red;
    } else if (turn == Turn::Red) {
        turn = Turn::Blue;
    }

    movements++;
}

/**
 * Connect a blue movement with its surrounding blue positions.
 *
 * @param row The row number.
 * @param col The column number.
 */
void Board::connectBlue(int row, int col)
{
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0)
                continue;

            if (! exists(row + r, col + c))
                continue;

            if (! isBlue(row + r, col + c))
                continue;

            blueGraph.connect(cell(row, col), cell(row + r, col + c));
        }
    }
}

/**
 * Connect a red movement with its surrounding red positions.
 *
 * @param row The row number.
 * @param col The column number.
 */
void Board::connectRed(int row, int col)
{
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0)
                continue;

            if (! exists(row + r, col + c))
                continue;

            if (! isRed(row + r, col + c))
                continue;

            redGraph.connect(cell(row, col), cell(row + r, col + c));
        }
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
bool Board::exists(int row, int col) const
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

/**
 * Get the row number of the rendered board that corresponds
 * to a given row and column of the board.
 *
 * @return Row number.
 */
int Board::getY(int row, int col) const
{
    return row*2;
}

/**
 * Get the column number of the rendered board that corresponds
 * to a given row and column of the board.
 *
 * @return Column number.
 */
int Board::getX(int row, int col) const
{
    return col*4 + row*2;
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
 * @return Color of the player that should do the next move.
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
 * Check if a position is owned by the blue player.
 *
 * @param row The row number.
 * @param col The column number.
 *
 * @return True if the position is owned by the blue player.
 */
bool Board::isBlue(int row, int col)
{
    Position position = std::make_pair(row, col);

    return positions[position] == Turn::Blue;
}

/**
 * Check if a position is owned by the red player.
 *
 * @param row The row number.
 * @param col The column number.
 *
 * @return True if the position is owned by the red player.
 */
bool Board::isRed(int row, int col)
{
    Position position = std::make_pair(row, col);

    return positions[position] == Turn::Red;
}

/**
 * Check if any of the player's already won.
 */
void Board::checkGame()
{
    if (blueDijkstra.nodesAreConnected(cell(0, 0), cell(size - 1, size - 1))) {
        winner = Turn::Blue;
        turn = Turn::Undecided;
    }

    if (redDijkstra.nodesAreConnected(cell(0, 0), cell(size - 1, size - 1))) {
        winner = Turn::Red;
        turn = Turn::Undecided;
    }
}

/**
 * Color of the player who won the game.
 *
 * @return Color of the player who won the game.
 *         If the game hasn't finished yet, the
 *         value "undecided" will be returned.
 */
Turn Board::playerWon()
{
    return winner;
}

/**
 * Set the given cell with the color of the current player.
 *
 * @param row The row number.
 * @param col The column number.
 */
void Board::set(int row, int col)
{
    if (turn == Turn::Undecided)
        throw std::runtime_error("The game already ended");

    Position position = std::make_pair(row, col);

    if (positions[position] != Turn::Undecided)
        throw std::invalid_argument("The cell has already been chosen");

    if (opening.first == -1 || opening.second == -1)
        opening = position;

    positions[position] = turn;

    if (turn == Turn::Blue)
        connectBlue(row, col);

    if (turn == Turn::Red)
        connectRed(row, col);

    checkGame();

    next();
}

/**
 * Get the color of a given cell.
 *
 * @param row The row number.
 * @param col The column number.
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
 * Get a read only version of the blue graph.
 *
 * @return Read only pointer to the blue graph.
 */
const Graph& Board::getBlueGraph() const {
    return blueGraph;
}

/**
 * Get a read only version of the red graph.
 *
 * @return Read only pointer to the red graph.
 */
const Graph& Board::getRedGraph() const {
    return redGraph;
}

/**
 * Facilitate iterating over the lines of the board
 * in order to print them or redirect them to the
 * standard output.
 *
 * @param callback Function that will be called back
 *        with the corresponding line.
 */
void Board::forEachLine(std::function<void(const char* line)> callback) const
{
    char line[128] = "";
    Turn piece;

    for (int row = 0; row < size; row++) {
        memset(line, 0, sizeof(line));

        for (int i = 0; i < row * 2; i++) {
            line[i] = ' ';
        }

        int pos = row * 2;

        for (int col = 0; col < size; col++) {
            line[pos++] = 'o';
            
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
 * Facilitate iterating over the pieces of the board
 * in order to print them or redirect them to the
 * standard output.
 *
 * @param callback Function that will be called back
 *        with the corresponding piece.
 */
void Board::forEachPiece(std::function<void(const int row, const int col, Turn turn)> callback) const
{
    Position position;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            position = std::make_pair(i, j);
            if (positions[position] != Turn::Undecided)
                callback(i, j, positions[position]);
        }
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