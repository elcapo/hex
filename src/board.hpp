#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <cstring>
#include <memory>
#include <utility> // para std::pair
#include "graph.hpp"
#include "dijkstra.hpp"
#include "common.hpp"
#include "strategy.hpp"

// Forward declarations
class MoveStrategy;

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
 * The `Positions` class is used to store the positions of the pieces.
 */
constexpr int MAX_BOARD_SIZE = 23;

class Positions {
private:
    int size;
    Turn positions[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

public:
    Positions(int size) : size(size) {
        std::fill(positions[0], positions[0] + (MAX_BOARD_SIZE * MAX_BOARD_SIZE), Turn::Undecided);
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

    // The humans playing the game
    HumanPlayers humanPlayers;

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

    // Strategies for computer players
    std::unique_ptr<MoveStrategy> blueStrategy;
    std::unique_ptr<MoveStrategy> redStrategy;

    /**
     * Connect the borders of each graph.
     */
    void connectBorders();

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
     * Let the computer play a move using the strategy pattern.
     */
    void playComputerMove();

    /**
     * Let the AI play a blue move.
     */
    void playBlueMove();

    /**
     * Let the AI play a red move.
     */
    void playRedMove();

public:
    /**
     * Basic constructor of a board.
     *
     * @param size Size of the size of the board.
     * @param humanPlayers Humans playing the game.
     *
     * @example Set size to 11 for a 11x11 board.
     */
    Board(int size, HumanPlayers humanPlayers);

    /**
     * Copy constructor for Board. Note that strategies are not copied.
     * 
     * @param other The board to copy from.
     */
    Board(const Board& other);

    /**
     * Assignment operator to allow copying one board to another. Copying boards
     * always results in a copy with AI disabled.
     * 
     * @param other The board to copy from.
     *
     * @return Reference to this board after assignment.
     */
    Board& operator=(const Board& other);

    /**
     * Set the strategy for the computer player.
     * 
     * @param player The player to set the strategy for.
     * @param strategy The strategy to use.
     */
    void setStrategy(Turn player, std::unique_ptr<MoveStrategy> strategy);

    /**
     * Check if any of the player's already won.
     */
    void checkGame();

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
     * Get the size of the board.
     *
     * @return Board size.
     */
    int getSize() const;

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
     * @param checkWinner Whether to check who won after the movement (default: true).
     */
    void set(int row, int col, bool checkWinner=true);

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
     * Facilitate iterating over the empty positions of the board
     * in order to fill them in Monte Carlo simulations.
     *
     * @param callback Function that will be called back
     *        with the corresponding empty position.
     */
    void forEachEmptyPosition(std::function<void(const int row, const int col)> callback) const;

    /**
     * Override the << operator in order to facilitate streaming the
     * board over a standard output.
     */
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

#endif // BOARD_H