#ifndef __BOARD_TEST__
#define __BOARD_TEST__

#include <gtest/gtest.h>

#include "../src/board.cpp"

TEST(BoardTests, existsEdge) {
    Board board(3);

    ASSERT_EQ(board.exists(0, 1), true);
}

#endif