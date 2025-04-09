#include <limits>
#include "ai.hpp"

bool flipCoin() {
    return rand() > (RAND_MAX / 2);
}

int BoardEvaluation::getScore(int row, int col)
{
    if (row < 0 || row >= size || col < 0 || col >= size)
        throw std::out_of_range("Row or column index is out of range.");

    return positions[row][col];
}

void BoardEvaluation::increaseScore(int row, int col)
{
    if (row < 0 || row >= size || col < 0 || col >= size)
        throw std::out_of_range("Row or column index is out of range.");

    positions[row][col]++;
}

void BoardEvaluation::decreaseScore(int row, int col)
{
    if (row < 0 || row >= size || col < 0 || col >= size)
        throw std::out_of_range("Row or column index is out of range.");

    positions[row][col]--;
}

void BoardEvaluation::deactivate(int row, int col)
{
    if (row < 0 || row >= size || col < 0 || col >= size)
        throw std::out_of_range("Row or column index is out of range.");

    positions[row][col] = std::numeric_limits<int>::min();
}

Position BoardEvaluation::getBestPosition()
{
    int bestValue = positions[0][0];
    Position bestPosition = {0, 0};

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (positions[i][j] < bestValue)
                continue;

            if (positions[i][j] == bestValue && flipCoin())
                continue;

            bestValue = positions[i][j];
            bestPosition = Position({i, j});
        }
    }

    return bestPosition;
}

void Ai::readBoard(const Board& externalBoard)
{
    board = externalBoard;
    evaluation = BoardEvaluation(board.getSize());
}

void Ai::simulate()
{
    Board simulation = board;

    // Randomize next movements until the board is completed
    if (simulation.countMovements() == 1 && flipCoin())
        simulation.pieRule();

    simulation.forEachEmptyPosition([&simulation] (const int row, const int col) {
        simulation.set(row, col, false);
    });

    // Check who won
    simulation.checkGame();

    // Evaluate the final position
    simulation.forEachPiece([&simulation, this] (const int row, const int col, Turn turn) {
        // If we won, we'll increase the score of our choices
        if (simulation.playerWon() == this->player && turn == this->player)
            this->evaluation.increaseScore(row, col);

        // If we losed, we'll decrease the score of our choices
        if (simulation.playerWon() != this->player && turn == this->player)
            this->evaluation.decreaseScore(row, col);
    });

    // Make sure initial positions are not considered
    board.forEachPiece([&simulation, this] (const int row, const int col, Turn turn) {
        this->evaluation.deactivate(row, col);
    });
}

Position Ai::getBestPosition()
{
    return evaluation.getBestPosition();
}