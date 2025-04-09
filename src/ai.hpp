#ifndef AI_H
#define AI_H

#include "board.hpp"

class BoardEvaluation
{
private:
    int size;
    int positions[MAX_BOARD_SIZE][MAX_BOARD_SIZE] = {0};

public:
    /**
     * Constructs a new board evaluation object.
     *
     * @param size The size of the board to be evaluated.
     */
    BoardEvaluation(int size) : size(size) {}

    /**
     * Retrieves the score at a specific position on the board.
     *
     * @param row The row index of the position.
     * @param col The column index of the position.
     *
     * @return The score at the specified position.
     */
    int getScore(int row, int col);

    /**
     * Increases the score at a specific position on the board by 1.
     *
     * @param row The row index of the position to be incremented.
     * @param col The column index of the position to be incremented.
     */
    void increaseScore(int row, int col);

    /**
     * Decreases the score at a specific position on the board by 1.
     *
     * @param row The row index of the position to be decremented.
     * @param col The column index of the position to be decremented.
     */
    void decreaseScore(int row, int col);

    /**
     * Deactivates the evaluation of positions (that have already been played).
     *
     * @param row The row index of the position to be deactivated.
     * @param col The column index of the position to be deactivated.
     */
    void deactivate(int row, int col);

    /**
     * Retrieves the best position on the board based on some evaluation criteria.
     *
     * @return Position The best position found on the board.
     */
    Position getBestPosition();
};

class Ai
{
private:
    Turn player;
    Board board;
    BoardEvaluation evaluation;

public:
    /**
     * Create an AI instance.
     */
    Ai(Turn player) :
        player(player),
        board(Board(MAX_BOARD_SIZE, HumanPlayers({true, true}))),
        evaluation(BoardEvaluation(MAX_BOARD_SIZE))
    {}

    /**
     * Read an external board state.
     *
     * @param externalBoard Board with the state to be read.
     */
    void readBoard(const Board& externalBoard);

    /**
     * Simulate a board match until its end.
     */
    void simulate();

    /**
     * Retrieves the best position on the board after the simulation
     * series finished.
     *
     * @return Position The best position found on the board.
     */
    Position getBestPosition();
};

#endif // AI_H