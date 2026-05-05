#pragma once

#include "Models.h"
#include "Graph.h"
#include "MatchingEngine.h"
#include <unordered_map>
#include <queue>
#include <mutex>

// Result of a ride request
struct RideResult {
    bool success;
    std::string message;
    std::string rideId;
    std::string driverId;
    std::string driverName;
    double distance;
    double fare;
    std::vector<std::string> path;
};

class RideManager {
private:
    Graph mapGraph;
    std::unordered_map<std::string, Driver> drivers;
    std::vector<Ride> rideHistory;
    
    // Request queue (FIFO)
    struct RideRequest {
        std::string pickupLocation;
        std::string destination;
    };
    std::queue<RideRequest> rideRequests;

    // Pricing constants
    const double BASE_FARE = 5.0;
    const double COST_PER_KM = 2.0;

    // Mutex for thread-safety (concurrency handling)
    mutable std::mutex systemMutex;

public:
    RideManager();

    void addLocationEdge(const std::string& u, const std::string& v, double distance);
    void addDriver(const Driver& driver);
    
    // Processes a ride request directly and returns the result (synchronous for API)
    RideResult processRideRequest(const std::string& pickupLocation, const std::string& destination);
    
    // API Helpers
    std::vector<Driver> getAllDrivers() const;
    std::vector<Ride> getRideHistory() const;
    void updateDriverLocation(const std::string& driverId, const std::string& newLocation);


private:
    double calculateFare(double distance);
    std::string generateRideId();
};
