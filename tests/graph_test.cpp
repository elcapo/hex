#ifndef __GRAPH_TEST__
#define __GRAPH_TEST__

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

TEST(GraphTests, countBidirectedEdges) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.bidirectedConnect(0, 1);
    graph.bidirectedConnect(1, 2);
    graph.bidirectedConnect(2, 0);

    ASSERT_EQ(graph.countEdges(), 6);
}

TEST(GraphTests, countDirectedEdges) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.directedConnect(0, 1);
    graph.directedConnect(1, 2);
    graph.directedConnect(2, 0);

    ASSERT_EQ(graph.countEdges(), 3);
}

#endif