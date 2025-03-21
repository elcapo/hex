#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "graph.hpp"

/**
 * The `Turn` enum contains the possible colors for each player.
 */
enum Turn { Undecided = 0, Blue = 1, Red = 2 };

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
 * The `Board` class models a board for the Hex game by implementing
 * it as a graph.
 */
class Board
{
private:
    // The number of rows and also the number of columns
    int size;

    // The color of the player that will do the next move
    Turn turn;

    // The number of movements that have been made
    int movements;

    // The opening position of the match
    Position opening;

    // The graph that represents cells and their connections
    Graph graph;

    // The piece positions at a given moment
    Positions positions;

    /**
     * Passes the turn to the next player
     */
    void next();
public:
    Board(int size) : size(size), turn(Turn::Blue), movements(0), opening({-1, -1}), graph(size*size, size*size*6), positions(size)
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
     *         Also can return -1, if the cell does not exist.
     */
    int cell(int row, int col);

    /**
     * Get the color of the current turn.
     */
    Turn current();

    /**
     * Set the given cell with the color of the current player.
     */
    void set(int row, int col);

    /**
     * Get the color of a given cell.
     */
    Turn get(int row, int col);

    /**
     * Apply the pie rule after the first movement, changing the color
     * of the first piece and passing the turn.
     */
    void pieRule();

    /**
     * Get a read only version of the graph.
     *
     * @return Read only pointer to the graph.
     */
    const Graph& getGraph() const;

    /**
     * Override the << operator in order to facilitate streaming the
     * board over a standard output.
     */
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

#endif // BOARD_H