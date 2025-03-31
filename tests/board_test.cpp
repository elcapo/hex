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

TEST(BoardTests, checkGame) {
    Board board(3);

    board.set(0, 0);
    board.set(1, 0);
    board.set(0, 1);
    board.set(1, 1);
    board.set(0, 2);

    ASSERT_EQ(board.playerWon(), Turn::Blue);
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

TEST(BoardTests, forEachLine)
{
    Board board(3);

    int i = 0;

    auto callback = [&i](const char* line) {
        const char* expectedLines[5] = {
            "o - o - o",
            " \\ / \\ / \\",
            "  o - o - o",
            "   \\ / \\ / \\",
            "    o - o - o",
        };

        ASSERT_EQ(*expectedLines[i], *line);

        i++;
    };

    board.forEachLine(callback);
}

TEST(BoardTests, forEachPiece)
{
    Board board(3);

    board.set(1, 1);
    board.pieRule();
    board.set(2, 1);

    int i = 0;

    struct expectation {
        int row, col;
        Turn turn;
    };

    auto callback = [&i](const int row, const int col, Turn turn) {
        expectation expectations[2];
        expectations[0] = {1, 1, Turn::Red};
        expectations[1] = {2, 1, Turn::Blue};

        ASSERT_EQ(expectations[i].row, row);
        ASSERT_EQ(expectations[i].col, col);
        ASSERT_EQ(expectations[i].turn, turn);

        i++;
    };

    board.forEachPiece(callback);
}

#endif // __BOARD_TEST__