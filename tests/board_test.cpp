#ifndef __BOARD_TEST__
#define __BOARD_TEST__

#include <gtest/gtest.h>
#include "../src/board.cpp"

TEST(BoardTests, exists) {
    Board board(3);

    ASSERT_EQ(board.exists(0, 0), true);
    ASSERT_EQ(board.exists(9, 9), false);
}

TEST(BoardTests, cell) {
    Board board(3);

    ASSERT_EQ(board.cell(0, 0), 0);
    ASSERT_EQ(board.cell(0, 1), 1);
    ASSERT_EQ(board.cell(1, 1), 4);
}

TEST(BoardTests, current) {
    Board board(3);

    ASSERT_EQ(board.current(), Turn::Blue);

    board.set(1, 1);

    ASSERT_EQ(board.current(), Turn::Red);
}


TEST(BoardTests, setGet) {
    Board board(3);

    ASSERT_EQ(board.get(1, 1), Turn::Undecided);

    board.set(1, 1);

    ASSERT_EQ(board.get(1, 1), Turn::Blue);
}

TEST(BoardTests, pieRule) {
    Board board(3);

    ASSERT_EQ(board.current(), Turn::Blue);

    board.set(1, 1);

    ASSERT_EQ(board.current(), Turn::Red);

    board.pieRule();

    ASSERT_EQ(board.get(1, 1), Turn::Red);
    ASSERT_EQ(board.current(), Turn::Blue);
}

#endif