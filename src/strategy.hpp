#ifndef STRATEGY_H
#define STRATEGY_H

#include "common.hpp"

// Forward declarations
class Board;
typedef std::pair<int, int> Position;
// enum Turn; // Ya está incluido en common.hpp

/**
 * Abstract strategy interface for game moves
 */
class MoveStrategy {
public:
    /**
     * Calculate the next move for a player
     * 
     * @param board Current game board state
     * @return Position The best position found for the next move
     */
    virtual Position getNextMove(const Board& board) = 0;
    
    /**
     * Virtual destructor for proper cleanup
     */
    virtual ~MoveStrategy() = default;
};

/**
 * AI implementation of the move strategy using Monte Carlo simulation
 */
class AIStrategy : public MoveStrategy {
private:
    Turn player;
    int simulationCount;
    
public:
    /**
     * Create an AI strategy for a specific player
     * 
     * @param player The player color (Blue or Red)
     * @param simulationCount Number of simulations to run (default: 100)
     */
    AIStrategy(Turn player, int simulationCount = 100) : 
        player(player), 
        simulationCount(simulationCount) {}

    /**
     * Calculate the next move using AI simulations
     * 
     * @param board Current game board state
     * @return Position The best position found for the next move
     */
    Position getNextMove(const Board& board) override;
};

#endif // STRATEGY_H