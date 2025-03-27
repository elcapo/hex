#include "graph.hpp"
#include "dijkstra.hpp"

/**
 * Constructor that preallocates memory based on graph size
 *
 * @param graph The graph to perform Dijkstra's algorithm on
 */
Dijkstra::Dijkstra(Graph& graph) : graph(graph) {
    int nodeCount = graph.countNodes();

    distances.resize(nodeCount, std::numeric_limits<int>::max());
    previous.resize(nodeCount, -1);
    visited.resize(nodeCount, false);
}

/**
 * Find the shortest path between two nodes
 *
 * @param start Starting node
 * @param end Destination node
 *
 * @return Vector of nodes representing the shortest path
 */
std::vector<int> Dijkstra::findShortestPath(int start, int end) {
    int nodeCount = graph.countNodes();

    if (start < 0 || start >= nodeCount || end < 0 || end >= nodeCount) {
        throw std::out_of_range("Invalid start or end node");
    }

    std::fill(distances.begin(), distances.end(), std::numeric_limits<int>::max());
    std::fill(previous.begin(), previous.end(), -1);
    std::fill(visited.begin(), visited.end(), false);

    std::priority_queue<int, std::vector<int>, DistanceCompare> queue((DistanceCompare(distances)));
    
    distances[start] = 0;
    queue.push(start);

    while (!queue.empty()) {
        int current = queue.top();
        queue.pop();

        if (current == end)
            break;

        if (visited[current])
            continue;

        visited[current] = true;

        graph.forEachEdgeFrom(current, [&](const Edge& edge) {
            if (visited[edge.to])
                return;

            int newDistance = distances[current] + 1;

            if (newDistance < distances[edge.to]) {
                distances[edge.to] = newDistance;
                previous[edge.to] = current;
                queue.push(edge.to);
            }
        });
    }

    std::vector<int> path;
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }

    std::reverse(path.begin(), path.end());

    return (path[0] == start) ? path : std::vector<int>();
}

/**
 * Get the total distance of the last computed shortest path
 *
 * @return Total distance of the path
 */
int Dijkstra::getPathDistance() const {
    // Assumes the last call to find shortest path used end node as index
    return distances.empty() ? -1 : distances.back();
}