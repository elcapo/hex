#ifndef __BOARD_TEST__
#define __BOARD_TEST__

#include <gtest/gtest.h>
#include "../src/board.cpp"

TEST(BoardTests, exists) {
    Board board(3);

    ASSERT_EQ(board.exists(0, 0), true);
    ASSERT_EQ(board.exists(9, 9), false);
}

TEST(BoardTests, getY) {
    Board board(3);

    ASSERT_EQ(board.getY(0, 0), 0);
    ASSERT_EQ(board.getY(0, 1), 0);
    ASSERT_EQ(board.getY(1, 0), 2);
    ASSERT_EQ(board.getY(1, 1), 2);
    ASSERT_EQ(board.getY(2, 2), 4);
}

TEST(BoardTests, getX) {
    Board board(3);

    ASSERT_EQ(board.getX(0, 0), 0);
    ASSERT_EQ(board.getX(0, 1), 4);
    ASSERT_EQ(board.getX(1, 0), 2);
    ASSERT_EQ(board.getX(1, 1), 6);
    ASSERT_EQ(board.getX(2, 2), 12);
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

TEST(BoardTests, countMovements) {
    Board board(3);

    ASSERT_EQ(board.countMovements(), 0);

    board.set(1, 1);

    ASSERT_EQ(board.countMovements(), 1);

    board.pieRule();

    ASSERT_EQ(board.countMovements(), 2);

    board.set(1, 2);

    ASSERT_EQ(board.countMovements(), 3);
}

TEST(BoardTests, set) {
    Board board(3);

    board.set(1, 1);

    ASSERT_EQ(board.get(1, 1), Turn::Blue);
}

TEST(BoardTests, get) {
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