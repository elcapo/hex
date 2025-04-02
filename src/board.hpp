#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <cstring>
#include "graph.hpp"
#include "dijkstra.hpp"

/**
 * The `Turn` enum contains the possible colors for each player.
 */
enum Turn { Undecided = 0, Blue = 1, Red = 2 };

/**
 * Get a given turn as a single character.
 *
 * @return 'B' for Turn::Blue
 *         'R' for Turn::Red
 *         '·' otherwise
 */
const char* turnAsChar(const Turn& t);

/**
 * Get a given turn as a printable label.
 *
 * @return 'Blue' for Turn::Blue
 *         'Red' for Turn::Red
 *         'Undecided' for Turn::Undecided
 */
std::string turnAsLabel(const Turn& t);

/**
 * Custom hash function for int.
 */
struct IntHash {
    size_t operator()(int n) const noexcept {
        return static_cast<size_t>(n);
    }
};

/**
 * Custom hash function for std::pair<int, int>.
 */
struct PairHash {
    template<typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& p) const noexcept {
        auto h1 = IntHash{}(p.first);
        auto h2 = IntHash{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

/**
 * The `Position` type is used to store a reference to a cell.
 */
typedef std::pair<int, int> Position;

/**
 * The `Positions` class is used to store the positions of the pieces.
 */
constexpr int MAX_BOARD_SIZE = 16;

class Positions {
private:
    int size;
    Turn positions[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

public:
    Positions(int size) : size(size) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                positions[i][j] = Turn::Undecided;
            }
        }
    }

    Turn& operator[](const Position& position) {
        return positions[position.first][position.second];
    }

    const Turn& operator[](const Position& position) const {
        return positions[position.first][position.second];
    }
};

/**
 * The `Board` class models a board for the Hex game.
 */
class Board
{
private:
    // The number of rows and also the number of columns
    int size;

    // The color of the player that will do the next move
    Turn turn;

    // The color of the player that won the game
    Turn winner;

    // The number of movements that have been made
    int movements;

    // The opening position of the match
    Position opening;

    // The piece positions at a given moment
    Positions positions;

    // The graph that represents cells and their connections
    Graph blueGraph;

    // The graph that represents cells and their connections
    Graph redGraph;

    // Dijkstra algorithm ready to evaluate the blue graph
    Dijkstra blueDijkstra;

    // Dijkstra algorithm ready to evaluate the red graph
    Dijkstra redDijkstra;

    /**
     * Passes the turn to the next player
     */
    void next();

    /**
     * Connect a blue movement with its surrounding blue positions.
     *
     * @param row The row number.
     * @param col The column number.
     */
    void connectBlue(int row, int col);

    /**
     * Connect a red movement with its surrounding red positions.
     *
     * @param row The row number.
     * @param col The column number.
     */
    void connectRed(int row, int col);

    /**
     * Check if any of the player's already won.
     */
    void checkGame();
public:
    /**
     * Basic constructor of a board.
     *
     * @param size Size of the size of the board.
     *
     * @example Set size to 11 for a 11x11 board.
     */
    Board(int size);

    /**
     * Creates a deep copy of the board with its current state.
     * 
     * @return A new Board object that is a complete copy of this board.
     */
    Board clone() const;

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
    bool exists(int row, int col) const;

    /**
     * Get the row number of the rendered board that corresponds
     * to a given row and column of the board.
     *
     * @return Row number.
     */
    int getY(int row, int col) const;

    /**
     * Get the column number of the rendered board that corresponds
     * to a given row and column of the board.
     *
     * @return Column number.
     */
    int getX(int row, int col) const;

    /**
     * Obtain the number of a cell counting from its top left
     * cell, in a first left then down order.
     *
     * @param row The row number.
     * @param col The column number.
     *
     * @return A number for each cell in a range from 0 to pow(size, 2)-1.
     *         Also can return -1, if the cell does not exist.
     */
    int cell(int row, int col) const;

    /**
     * Get the color of the current turn.
     *
     * @return Color of the player that should do the next move.
     */
    const Turn& current() const;

    /**
     * Get the number of movements that have been played.
     *
     * @return Number of movements.
     */
    const int& countMovements() const;

    /**
     * Check if a position is owned by the blue player.
     *
     * @param row The row number.
     * @param col The column number.
     *
     * @return True if the position is owned by the blue player.
     */
    bool isBlue(int row, int col);

    /**
     * Check if a position is owned by the red player.
     *
     * @param row The row number.
     * @param col The column number.
     *
     * @return True if the position is owned by the red player.
     */
    bool isRed(int row, int col);

    /**
     * Color of the player who won the game.
     *
     * @return Color of the player who won the game.
     *         If the game hasn't finished yet, the
     *         value "undecided" will be returned.
     */
    Turn playerWon();

    /**
     * Set the given cell with the color of the current player.
     *
     * @param row The row number.
     * @param col The column number.
     */
    void set(int row, int col);

    /**
     * Get the color of a given cell.
     *
     * @param row The row number.
     * @param col The column number.
     */
    const Turn& get(int row, int col) const;

    /**
     * Apply the pie rule after the first movement, changing the color
     * of the first piece and passing the turn.
     */
    void pieRule();

    /**
     * Get a read only version of the blue graph.
     *
     * @return Read only pointer to the blue graph.
     */
    const Graph& getBlueGraph() const;

    /**
     * Get a read only version of the red graph.
     *
     * @return Read only pointer to the red graph.
     */
    const Graph& getRedGraph() const;

    /**
     * Facilitate iterating over the lines of the board
     * in order to print them or redirect them to the
     * standard output.
     *
     * @param callback Function that will be called back
     *        with the corresponding line.
     */
    void forEachLine(std::function<void(const char* line)> callback) const;

    /**
     * Facilitate iterating over the pieces of the board
     * in order to print them or redirect them to the
     * standard output.
     *
     * @param callback Function that will be called back
     *        with the corresponding piece.
     */
    void forEachPiece(std::function<void(const int row, const int col, Turn turn)> callback) const;

    /**
     * Override the << operator in order to facilitate streaming the
     * board over a standard output.
     */
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

#endif // BOARD_H