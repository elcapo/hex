#ifndef __GRAPH_TEST__
#define __GRAPH_TEST__

#include <gtest/gtest.h>
#include "../src/graph.cpp"

TEST(GraphTests, countNodes) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    ASSERT_EQ(graph.countNodes(), nodes);
}

TEST(GraphTests, connect) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.connect(0, 1, false);

    ASSERT_EQ(graph.countEdges(), 1);
}

TEST(GraphTests, bidirectedConnect) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.bidirectedConnect(0, 1);
    graph.bidirectedConnect(1, 2);
    graph.bidirectedConnect(2, 0);

    ASSERT_EQ(graph.countEdges(), 6);
}

TEST(GraphTests, directedConnect) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.directedConnect(0, 1);
    graph.directedConnect(1, 2);
    graph.directedConnect(2, 0);

    ASSERT_EQ(graph.countEdges(), 3);
}

TEST(GraphTests, nodesAreConnected) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.bidirectedConnect(0, 1);
    graph.directedConnect(1, 2);

    ASSERT_EQ(graph.nodesAreConnected(0, 1), true);
    ASSERT_EQ(graph.nodesAreConnected(1, 0), true);
    ASSERT_EQ(graph.nodesAreConnected(1, 2), true);
    ASSERT_EQ(graph.nodesAreConnected(2, 1), false);
}

TEST(GraphTests, forEachEdgeFrom)
{
    std::vector<Edge> calledEdges;

    auto callback = [&calledEdges](const Edge edge) {
        calledEdges.push_back(edge);
    };

    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.directedConnect(0, 1);
    graph.directedConnect(0, 2);
    graph.directedConnect(1, 3);
    graph.forEachEdgeFrom(0, callback);

    std::vector<int> expected = {1, 2};
    for (int i = 0; i < expected.size(); i++) {
        ASSERT_EQ(calledEdges[i].to, expected[i]);
    }
}

TEST(GraphTests, forEachEdge)
{
    std::vector<Edge> calledEdges;

    auto callback = [&calledEdges](const Edge edge) {
        calledEdges.push_back(edge);
    };

    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.directedConnect(0, 1);
    graph.directedConnect(0, 2);
    graph.directedConnect(1, 3);
    graph.forEachEdge(callback);

    std::vector<int> expected = {1, 2, 3};
    for (int i = 0; i < expected.size(); i++) {
        ASSERT_EQ(calledEdges[i].to, expected[i]);
    }
}

#endif