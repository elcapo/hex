#include "graph.hpp"

/**
 * If necessary, allocates more memory when connections are created.
 */
void Graph::reserveIfNeeded()
{
    if (edgeCapacity == countEdges()) {
        edgeCapacity = edgeCapacity * edgeCapacity/2;
        connections.reserve(edgeCapacity);
    }
}

/**
 * Connects two nodes with an edge.
 *
 * @param from The source node of the connection.
 * @param to The target node of the connection.
 * @param bidirectional Whether the connection is bidirectional.
 */
void Graph::connect(int from, int to, bool bidirectional)
{
    reserveIfNeeded();

    if (from < 0 || from > nodes) {
        throw std::out_of_range(
            "Can't create the connection because the from is invalid"
        );
    }

    if (to < 0 || to > nodes) {
        throw std::out_of_range(
            "Can't create the connection because the target is invalid"
        );
    }

    if (nodesAreConnected(from, to)) {
        if (! bidirectional)
            return;

        if (nodesAreConnected(to, from))
            return;
    }

    connections.push_back({to, -1});

    int edge = countEdges() - 1;

    if (limits[from].head == -1) {
        limits[from].head = edge;
    } else {
        connections[limits[from].tail].next = edge;
    }

    limits[from].tail = edge;

    if (bidirectional) {
        connect(to, from, false);
    }
}

/**
 * Connects two nodes with a bidirected edge.
 *
 * @param from The source node of the connection.
 * @param to The target node of the connection.
 */
void Graph::bidirectedConnect(int from, int to)
{
    connect(from, to, true);
}

/**
 * Connects two nodes with a directed edge.
 *
 * @param from The source node of the connection.
 * @param to The target node of the connection.
 */
void Graph::directedConnect(int from, int to)
{
    connect(from, to, false);
}

/**
 * Check if two nodes are connected.
 *
 * @param from The source node of the connection.
 * @param to The target node of the connection.
 *
 * @return Whether the two nodes are connected.
 */
bool Graph::nodesAreConnected(int from, int to)
{
    if (from < 0 || from >= nodes || to < 0 || to >= nodes) {
        return false;
    }

    for (int i = limits[from].head; i != -1; i = connections[i].next) {
        if (connections[i].target == to) {
            return true;
        }
    }

    return false;
}

/**
 * Get the number of nodes.
 *
 * @return Number of nodes.
 */
int Graph::countNodes() const
{
    return nodes;
}

/**
 * Get the number of edges.
 *
 * Bidirected edges are counted as two directed edges.
 *
 * @return Number of edges.
 */
int Graph::countEdges() const
{
    return connections.size();
}

/**
 * Iterate each of the edges of a given node calling a callback.
 */
void Graph::forEachEdgeFrom(int node, std::function<void(const Edge&)> callback) const
{
    for (int edge = limits[node].head; edge != -1; edge = connections[edge].next) {
        callback(Edge(node, connections[edge].target));
    }
}

/**
 * Iterate each of the edges of the graph calling a callback.
 */
void Graph::forEachEdge(std::function<void(const Edge&)> callback) const
{
    for (int node = 0; node < nodes; ++node) {
        forEachEdgeFrom(node, callback);
    }
}

/**
 * Override the << operator in order to facilitate streaming the
 * board over a standard output.
 */
std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
    graph.forEachEdge([&os](const Edge& edge) {
        os << "(" << edge.from << ") > (" << edge.to << ")" << std::endl;
    });

    return os;
}