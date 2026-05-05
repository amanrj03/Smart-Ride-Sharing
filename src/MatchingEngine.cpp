#include "MatchingEngine.h"
#include <queue>
#include <limits>
#include <iostream>

std::string MatchingEngine::findNearestDriver(
    const std::string& riderLocation,
    const std::vector<Driver>& availableDrivers,
    const Graph& graph,
    double& outDistance
) {
    // 1. Compute shortest paths from Rider's location to all other nodes in the graph
    std::unordered_map<std::string, double> distances = graph.dijkstra(riderLocation);

    // 2. Use a Min Heap to find the nearest driver
    // Pair stores: {distance_to_rider, driver_id}
    using pdi = std::pair<double, std::string>;
    std::priority_queue<pdi, std::vector<pdi>, std::greater<pdi>> minHeap;

    for (const Driver& driver : availableDrivers) {
        if (!driver.isAvailable) continue; // Only consider available drivers
        
        // Check if there is a path to the driver's location
        if (distances.find(driver.currentLocation) != distances.end()) {
            double dist = distances[driver.currentLocation];
            // If it's a valid distance (not infinity)
            if (dist != std::numeric_limits<double>::infinity()) {
                minHeap.push({dist, driver.driverId});
            }
        }
    }

    if (minHeap.empty()) {
        outDistance = -1.0;
        return ""; // No driver found
    }

    // 3. Extract the nearest driver
    outDistance = minHeap.top().first;
    return minHeap.top().second;
}
