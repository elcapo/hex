#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stdexcept>
#include <functional>
#include <ostream>
#include <sstream>

/**
 * The Edge struct is used to model the connections of the graph
 * and it's used in its method to iterate edges.
 */
struct Edge {
    int from, to;
    Edge(int from, int to) : from(from), to(to) {}
};

/**
 * The NodeLimit struct is used to model the addresses of the head
 * and tail connection for each node.
 */
struct NodeLimit
{
    int head, tail;
    NodeLimit(int head, int tail) : head(head), tail(tail) {}
};

/**
 * The NodeConnection struct contains information about the target of a connection
 * and the address of the next connection of the same source.
 */
struct NodeConnection
{
    int target, next;
    NodeConnection(int target, int next) : target(target), next(next) {}
};

/**
 * The Graph class represents a collection of nodes and its connections.
 *
 * In order to keep it simple, it doesn't store weights of the vertices
 * as it assumes that all of them weight the same.
 *
 * The number of nodes can only be set on construction.
 *
 * Connections can be added but not removed.
 *
 */
class Graph {
private:
    // Number of nodes in the graph
    int nodes;

    // Edge capacity (applies to target and next vectors)
    int edgeCapacity;

    // Address to the head and tail edges of each node
    std::vector<NodeLimit> limits;

    // Edge targets and their next siblings
    std::vector<NodeConnection> connections;
public:
    /**
     * Constructor for the Graph class.
     *
     * @param nodes Number of nodes.
     * @param edgeCapacity Preallocated number of edges.
     */
    Graph(int nodes, int edgeCapacity);

    /**
     * If necessary, allocates more memory when connections are created.
     */
    void reserveIfNeeded();

    /**
     * Connects two nodes with an edge.
     *
     * @param from The source node of the connection.
     * @param to The target node of the connection.
     * @param bidirectional Whether the connection is bidirectional.
     */
    void connect(int from, int to, bool bidirectional = true);

    /**
     * Connects two nodes with a bidirected edge.
     *
     * @param from The source node of the connection.
     * @param to The target node of the connection.
     */
    void bidirectedConnect(int from, int to);

    /**
     * Connects two nodes with a directed edge.
     *
     * @param from The source node of the connection.
     * @param to The target node of the connection.
     */
    void directedConnect(int from, int to);

    /**
     * Check if two nodes are connected.
     *
     * @param from The source node of the connection.
     * @param to The target node of the connection.
     *
     * @return Whether the two nodes are connected.
     */
    bool nodesAreConnected(int from, int to);

    /**
     * Get the number of nodes.
     *
     * @return Number of nodes.
     */
    const int& countNodes() const;

    /**
     * Get the number of edges.
     *
     * Bidirected edges are counted as two directed edges.
     *
     * @return Number of edges.
     */
    const int countEdges() const;

    /**
     * Iterate each of the edges of a given node calling a callback.
     */
    void forEachEdgeFrom(int node, std::function<void(const Edge&)> callback) const;

    /**
     * Iterate each of the edges of the graph calling a callback.
     */
    void forEachEdge(std::function<void(const Edge&)> callback) const;

    /**
     * Return the graph as a string.
     *
     * @return String representation of the graph.
     */
    std::string toString() const;

    /**
     * Override the << operator in order to facilitate streaming the
     * graph over a standard output.
     */
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
};

#endif // GRAPH_H