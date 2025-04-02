#include "graph.hpp"

void Graph::reserveIfNeeded()
{
    if (edgeCapacity == countEdges()) {
        edgeCapacity = edgeCapacity * edgeCapacity/2;
        connections.reserve(edgeCapacity);
    }
}

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

void Graph::bidirectedConnect(int from, int to)
{
    connect(from, to, true);
}

void Graph::directedConnect(int from, int to)
{
    connect(from, to, false);
}

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

const int& Graph::countNodes() const
{
    return nodes;
}

const int Graph::countEdges() const
{
    return connections.size();
}

void Graph::forEachEdgeFrom(int node, std::function<void(const Edge&)> callback) const
{
    for (int edge = limits[node].head; edge != -1; edge = connections[edge].next)
        callback(Edge(node, connections[edge].target));
}

void Graph::forEachEdge(std::function<void(const Edge&)> callback) const
{
    for (int node = 0; node < nodes; ++node)
        forEachEdgeFrom(node, callback);
}

std::string Graph::toString() const
{
    std::ostringstream os;

    forEachEdge([&os](const Edge& edge) {
        os << "(" << edge.from << ") > (" << edge.to << ")" << std::endl;
    });

    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
    os << graph.toString();

    return os;
}