#ifndef __DIJKSTRA_TEST__
#define __DIJKSTRA_TEST__

#include <gtest/gtest.h>
#include "../src/dijkstra.hpp"

TEST(DijkstraTests, findShortestPath) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.bidirectedConnect(0, 1);
    graph.bidirectedConnect(1, 2);
    graph.bidirectedConnect(2, 5);
    graph.bidirectedConnect(5, 8);
    graph.bidirectedConnect(0, 4);
    graph.bidirectedConnect(4, 8);

    Dijkstra dijkstra(graph);

    std::vector<int> shortestPath;

    shortestPath = dijkstra.findShortestPath(0, 8);

    ASSERT_EQ(shortestPath[0], 0);
    ASSERT_EQ(shortestPath[1], 4);
    ASSERT_EQ(shortestPath[2], 8);
}

TEST(DijkstraTests, impossiblePath) {
    Graph graph(3, 3*2);

    graph.bidirectedConnect(0, 1);

    Dijkstra dijkstra(graph);

    std::vector<int> shortestPath;

    shortestPath = dijkstra.findShortestPath(0, 2);

    ASSERT_EQ(shortestPath.size(), 0);
}

TEST(DijkstraTests, nodesAreConnected) {
    int nodes = 3*3;
    Graph graph(nodes, nodes*6);

    graph.bidirectedConnect(0, 1);
    graph.bidirectedConnect(1, 2);
    graph.bidirectedConnect(2, 5);
    graph.bidirectedConnect(5, 8);
    graph.bidirectedConnect(0, 4);
    graph.bidirectedConnect(4, 8);

    Dijkstra dijkstra(graph);

    ASSERT_EQ(dijkstra.nodesAreConnected(0, 8), true);
    ASSERT_EQ(dijkstra.nodesAreConnected(0, 7), false);
}

#endif // __DIJKSTRA_TEST__