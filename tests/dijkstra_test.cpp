#ifndef __DIJKSTRA_TEST__
#define __DIJKSTRA_TEST__

#include <gtest/gtest.h>
#include "../src/dijkstra.cpp"

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

TEST(DijkstraTests, getPathDistance) {
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

    ASSERT_EQ(dijkstra.getPathDistance(), 2);
}

#endif // __DIJKSTRA_TEST__