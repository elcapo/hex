#ifndef __BOARD_TEST__
#define __BOARD_TEST__

#include <gtest/gtest.h>
#include <unordered_set>
#include <utility>
#include "../src/board.cpp"

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

TEST(BoardTests, copy) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(1, 1);
    board.pieRule();
    board.set(1, 2);

    Board copy = board;

    ASSERT_EQ(copy.exists(0, 0), true);
    ASSERT_EQ(copy.exists(9, 9), false);
    ASSERT_EQ(copy.isRed(0, 0), false);
    ASSERT_EQ(copy.isBlue(0, 0), false);
    ASSERT_EQ(copy.isRed(1, 1), true);
    ASSERT_EQ(copy.isBlue(1, 2), true);
}

TEST(BoardTests, exists) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.exists(0, 0), true);
    ASSERT_EQ(board.exists(9, 9), false);
}

TEST(BoardTests, getSize) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.getSize(), 3);
}

TEST(BoardTests, getY) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.getY(0, 0), 0);
    ASSERT_EQ(board.getY(0, 1), 0);
    ASSERT_EQ(board.getY(1, 0), 2);
    ASSERT_EQ(board.getY(1, 1), 2);
    ASSERT_EQ(board.getY(2, 2), 4);
}

TEST(BoardTests, getX) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.getX(0, 0), 0);
    ASSERT_EQ(board.getX(0, 1), 4);
    ASSERT_EQ(board.getX(1, 0), 2);
    ASSERT_EQ(board.getX(1, 1), 6);
    ASSERT_EQ(board.getX(2, 2), 12);
}

TEST(BoardTests, cell) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.cell(0, 0), 0);
    ASSERT_EQ(board.cell(0, 1), 1);
    ASSERT_EQ(board.cell(1, 1), 4);
    ASSERT_EQ(board.cell(2, 2), 8);
}

TEST(BoardTests, current) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.current(), Turn::Blue);

    board.set(1, 1);

    ASSERT_EQ(board.current(), Turn::Red);
}

TEST(BoardTests, countMovements) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.countMovements(), 0);

    board.set(1, 1);

    ASSERT_EQ(board.countMovements(), 1);

    board.pieRule();

    ASSERT_EQ(board.countMovements(), 2);

    board.set(1, 2);

    ASSERT_EQ(board.countMovements(), 3);
}

TEST(BoardTests, isBlue) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(1, 1);

    ASSERT_EQ(board.isBlue(0, 0), false);
    ASSERT_EQ(board.isBlue(1, 1), true);
}

TEST(BoardTests, isRed) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(0, 0);
    board.set(1, 1);

    ASSERT_EQ(board.isRed(0, 0), false);
    ASSERT_EQ(board.isRed(1, 1), true);
}

TEST(BoardTests, playerWon) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(0, 0);
    board.set(1, 0);
    board.set(0, 1);
    board.set(1, 1);
    board.set(0, 2);

    ASSERT_EQ(board.playerWon(), Turn::Blue);
}

TEST(BoardTests, set) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(1, 1);

    ASSERT_EQ(board.get(1, 1), Turn::Blue);
}

TEST(BoardTests, get) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.get(1, 1), Turn::Undecided);

    board.set(1, 1);

    ASSERT_EQ(board.get(1, 1), Turn::Blue);
}

TEST(BoardTests, pieRule) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    ASSERT_EQ(board.current(), Turn::Blue);

    board.set(1, 1);

    ASSERT_EQ(board.current(), Turn::Red);

    board.pieRule();

    ASSERT_EQ(board.get(1, 1), Turn::Red);
    ASSERT_EQ(board.current(), Turn::Blue);
}

TEST(BoardTests, getBlueGraph) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(0, 0);
    board.set(1, 1);
    board.set(0, 1);
    board.set(1, 2);

    Graph blueGraph = board.getBlueGraph();

    ASSERT_EQ(blueGraph.nodesAreConnected(0, 1), true);
}

TEST(BoardTests, getRedGraph) {
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(0, 0);
    board.set(1, 1);
    board.set(0, 1);
    board.set(1, 2);

    Graph redGraph = board.getRedGraph();

    ASSERT_EQ(redGraph.nodesAreConnected(4, 5), true);
}

TEST(BoardTests, forEachLine)
{
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

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
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

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

TEST(BoardTests, forEachEmptyPosition)
{
    HumanPlayers humanPlayers = {true, true};
    Board board(3, humanPlayers);

    board.set(1, 1);
    board.pieRule();
    board.set(2, 1);

    std::unordered_set<std::pair<int, int>, pair_hash> expectedPositions;

    expectedPositions.insert({0, 0});
    expectedPositions.insert({0, 1});
    expectedPositions.insert({0, 2});
    expectedPositions.insert({1, 0});
    expectedPositions.insert({1, 2});
    expectedPositions.insert({2, 0});
    expectedPositions.insert({2, 2});

    std::unordered_set<std::pair<int, int>, pair_hash> visitedPositions;

    board.forEachEmptyPosition([&visitedPositions](const int row, const int col) {
        visitedPositions.insert({row, col});
    });

    EXPECT_EQ(visitedPositions.size(), expectedPositions.size());
}


TEST(BoardTests, aiPlayer)
{
    HumanPlayers humanPlayers = {true, false};
    Board board(3, humanPlayers);

    board.set(1, 1);
    ASSERT_EQ(board.countMovements(), 2);
}

#endif // __BOARD_TEST__