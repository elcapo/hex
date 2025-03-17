#include <gtest/gtest.h>
#include "../src/graph.cpp"

TEST(GraphTests, countNodes) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    ASSERT_EQ(graph.countNodes(), nodes);
}

TEST(GraphTests, countEdges) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.connect(0, 1, false);

    ASSERT_EQ(graph.countEdges(), 1);
}