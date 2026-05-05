#pragma once

#include "Models.h"
#include "Graph.h"
#include <vector>
#include <string>

class MatchingEngine {
public:
    // Finds the nearest available driver using Dijkstra's output and a Min Heap
    // Returns driver ID if found, empty string if no available driver is reachable
    static std::string findNearestDriver(
        const std::string& riderLocation,
        const std::vector<Driver>& availableDrivers,
        const Graph& graph,
        double& outDistance
    );
};
