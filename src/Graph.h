#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

// Represents an edge (distance) to a neighboring node
struct Edge {
    std::string targetNode;
    double distance;
};

class Graph {
private:
    // Adjacency list: Node -> List of Edges
    std::unordered_map<std::string, std::vector<Edge>> adjList;

public:
    // Add a bi-directional road between two locations
    void addEdge(const std::string& u, const std::string& v, double distance);

    // Dijkstra's Algorithm: Returns a map of shortest distances from source to all other nodes
    std::unordered_map<std::string, double> dijkstra(const std::string& sourceNode) const;

    // Get the shortest path as a sequence of nodes
    std::vector<std::string> getShortestPath(const std::string& sourceNode, const std::string& targetNode) const;

    // Checks if a path exists between two nodes
    bool isConnected(const std::string& sourceNode, const std::string& targetNode) const;
    
    // Prints the graph
    void printGraph() const;
};
