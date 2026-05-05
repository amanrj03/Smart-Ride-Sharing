#include "Graph.h"
#include <iostream>
#include <queue>
#include <algorithm>

void Graph::addEdge(const std::string& u, const std::string& v, double distance) {
    adjList[u].push_back({v, distance});
    adjList[v].push_back({u, distance}); // Assuming undirected graph for roads
}

std::unordered_map<std::string, double> Graph::dijkstra(const std::string& sourceNode) const {
    std::unordered_map<std::string, double> distances;
    
    // Initialize all distances to infinity
    for (const auto& pair : adjList) {
        distances[pair.first] = std::numeric_limits<double>::infinity();
    }
    
    // If source doesn't exist in graph, return empty or just infinity
    if (adjList.find(sourceNode) == adjList.end()) {
        return distances;
    }

    distances[sourceNode] = 0.0;
    
    // Min Heap for Dijkstra (distance, node_name)
    using pdi = std::pair<double, std::string>;
    std::priority_queue<pdi, std::vector<pdi>, std::greater<pdi>> pq;
    
    pq.push({0.0, sourceNode});
    
    while (!pq.empty()) {
        auto [currDist, u] = pq.top();
        pq.pop();
        
        if (currDist > distances[u]) continue;
        
        for (const Edge& edge : adjList.at(u)) {
            const std::string& v = edge.targetNode;
            double weight = edge.distance;
            
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                pq.push({distances[v], v});
            }
        }
    }
    
    return distances;
}

std::vector<std::string> Graph::getShortestPath(const std::string& sourceNode, const std::string& targetNode) const {
    std::unordered_map<std::string, double> distances;
    std::unordered_map<std::string, std::string> previous;
    std::vector<std::string> path;

    for (const auto& pair : adjList) {
        distances[pair.first] = std::numeric_limits<double>::infinity();
    }

    if (adjList.find(sourceNode) == adjList.end()) return path;

    distances[sourceNode] = 0.0;

    using pdi = std::pair<double, std::string>;
    std::priority_queue<pdi, std::vector<pdi>, std::greater<pdi>> pq;

    pq.push({0.0, sourceNode});

    while (!pq.empty()) {
        auto [currDist, u] = pq.top();
        pq.pop();

        if (currDist > distances[u]) continue;

        if (u == targetNode) break;

        for (const Edge& edge : adjList.at(u)) {
            const std::string& v = edge.targetNode;
            double weight = edge.distance;

            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.push({distances[v], v});
            }
        }
    }

    if (distances[targetNode] == std::numeric_limits<double>::infinity()) {
        return path;
    }

    std::string current = targetNode;
    while (current != sourceNode) {
        path.push_back(current);
        current = previous[current];
    }
    path.push_back(sourceNode);
    std::reverse(path.begin(), path.end());

    return path;
}

bool Graph::isConnected(const std::string& sourceNode, const std::string& targetNode) const {
    auto distances = dijkstra(sourceNode);
    return distances.find(targetNode) != distances.end() && 
           distances[targetNode] != std::numeric_limits<double>::infinity();
}

void Graph::printGraph() const {
    for (const auto& pair : adjList) {
        std::cout << "Node " << pair.first << " connects to: ";
        for (const auto& edge : pair.second) {
            std::cout << "(" << edge.targetNode << ", " << edge.distance << ") ";
        }
        std::cout << "\n";
    }
}
