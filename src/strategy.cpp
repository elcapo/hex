#include "strategy.hpp"
#include "ai.hpp"
#include "board.hpp"

Position AIStrategy::getNextMove(const Board& board) {
    // Use the existing AI code to calculate the best move
    Ai ai(player);
    ai.readBoard(board);

    // Run simulations to determine the best move
    for (int i = 0; i <= simulationCount; i++) {
        ai.simulate();
    }

    // Return the best position found by the AI
    return ai.getBestPosition();
}