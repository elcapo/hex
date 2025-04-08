#include "graph.hpp"
#include "dijkstra.hpp"

void Dijkstra::adjust()
{
    int nodeCount = graph.countNodes();

    if (distances.capacity() >= nodeCount)
        return;

    distances.resize(nodeCount, std::numeric_limits<int>::max());
    previous.resize(nodeCount, -1);
    visited.resize(nodeCount, false);

    std::pair<int, int> distancePair = std::make_pair(0, -1);
    queueContainer.resize(nodeCount, distancePair);

    queue = std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        DistanceCompare>(comparator, std::move(queueContainer));

    int edgeCount = graph.countEdges();
    path.resize(edgeCount);
}

Dijkstra::Dijkstra(Graph& graph) : graph(graph), comparator() {
    adjust();
}

Dijkstra& Dijkstra::operator=(const Dijkstra& other)
{
    if (this == &other) {
        return *this;
    }

    graph = other.graph;
    adjust();

    return *this;
}

std::vector<int> Dijkstra::findShortestPath(int start, int end) {
    int nodeCount = graph.countNodes();

    if (start < 0 || start >= nodeCount || end < 0 || end >= nodeCount) {
        throw std::out_of_range("Invalid start or end node");
    }

    std::fill(distances.begin(), distances.end(), std::numeric_limits<int>::max());
    std::fill(previous.begin(), previous.end(), -1);
    std::fill(visited.begin(), visited.end(), false);

    queueContainer.clear();

    distances[start] = 0;

    queue.push(std::make_pair(0, start));

    while (! queue.empty()) {
        int current = queue.top().second;

        queue.pop();

        if (current == end)
            break;

        if (current == -1 || visited[current])
            continue;

        visited[current] = true;

        graph.forEachEdgeFrom(current, [&](const Edge& edge) {
            if (visited[edge.to])
                return;

            int newDistance = distances[current] + 1;

            if (newDistance < distances[edge.to]) {
                distances[edge.to] = newDistance;
                previous[edge.to] = current;
                queue.push(std::make_pair(newDistance, edge.to));
            }
        });
    }

    path.clear();
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }

    std::reverse(path.begin(), path.end());

    return (path[0] == start) ? path : std::vector<int>();
}

bool Dijkstra::nodesAreConnected(int start, int end) {
    std::vector<int> path = findShortestPath(start, end);

    return path.size() > 0;
}