#ifndef __AI_TEST__
#define __AI_TEST__

#include <gtest/gtest.h>
#include "../src/ai.hpp"

TEST(AiTests, getScore) {
    BoardEvaluation evaluation(9);

    ASSERT_EQ(evaluation.getScore(1, 1), 0);

    evaluation.increaseScore(1, 1);

    ASSERT_EQ(evaluation.getScore(1, 1), 1);
}

TEST(AiTests, increaseScore) {
    BoardEvaluation evaluation(9);

    evaluation.increaseScore(1, 1);
    evaluation.increaseScore(2, 2);
    evaluation.increaseScore(1, 1);

    ASSERT_EQ(evaluation.getScore(0, 0), 0);
    ASSERT_EQ(evaluation.getScore(1, 1), 2);
    ASSERT_EQ(evaluation.getScore(2, 2), 1);
}

TEST(AiTests, getBestPosition) {
    BoardEvaluation evaluation(9);

    evaluation.increaseScore(1, 1);
    evaluation.increaseScore(2, 2);
    evaluation.increaseScore(1, 1);

    ASSERT_EQ(evaluation.getBestPosition(), Position({1, 1}));
}

TEST(AiTests, simulate) {
    Ai ai(Turn::Blue);
    Board board(3, HumanPlayers({true, true}));

    board.set(0, 0);
    board.set(2, 0);
    board.set(0, 1);
    board.set(2, 1);
    board.set(1, 0);
    board.set(2, 2);
    board.set(1, 1);
    board.set(1, 2);

    ai.readBoard(board);
    ai.simulate();

    ASSERT_EQ(ai.getBestPosition(), Position({0, 2}));
}

#endif // __AI_TEST__