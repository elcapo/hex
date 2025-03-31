#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include "graph.hpp"

class Dijkstra {
private:
    Graph& graph;

    // Preallocated data structures to minimize memory allocations
    std::vector<int> distances;
    std::vector<int> previous;
    std::vector<bool> visited;
    std::vector<int> path;
    std::vector<std::pair<int, int>> queueContainer;

    struct DistanceCompare {
        bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
            return a.first > b.first;
        }
    };

    DistanceCompare comparator;

    std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        DistanceCompare> queue;
public:
    /**
     * Constructor that preallocates memory based on graph size
     *
     * @param graph The graph to perform Dijkstra's algorithm on
     */
    Dijkstra(Graph& graph);

    /**
     * Find the shortest path between two nodes
     *
     * @param start Starting node
     * @param end Destination node
     *
     * @return Vector of nodes representing the shortest path
     */
    std::vector<int> findShortestPath(int start, int end);

    /**
     * Check if two nodes are connected.
     *
     * @return Whether the nodes are connected.
     */
    bool nodesAreConnected(int start, int end);
};

#endif // DIJKSTRA_H