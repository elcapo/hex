#include <gtest/gtest.h>

#include "graph_test.cpp"
#include "board_test.cpp"
#include "dijkstra_test.cpp"
#include "ai_test.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}