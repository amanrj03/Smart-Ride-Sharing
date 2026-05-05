#include "RideManager.h"
#include <iostream>
#include <vector>

RideManager::RideManager() {}

void RideManager::addLocationEdge(const std::string& u, const std::string& v, double distance) {
    std::lock_guard<std::mutex> lock(systemMutex);
    mapGraph.addEdge(u, v, distance);
}

void RideManager::addDriver(const Driver& driver) {
    std::lock_guard<std::mutex> lock(systemMutex);
    drivers[driver.driverId] = driver;
}


RideResult RideManager::processRideRequest(const std::string& pickupLocation, const std::string& destination) {
    std::lock_guard<std::mutex> lock(systemMutex);


    // 1. Check if graph has a path to destination (disconnected graph edge case)
    if (!mapGraph.isConnected(pickupLocation, destination)) {
        return {false, "Cannot reach destination '" + destination + "' from '" + pickupLocation + "'.", "", "", "", 0.0, 0.0, {}};
    }

    // 2. Extract available drivers
    std::vector<Driver> availableDrivers;
    for (const auto& pair : drivers) {
        if (pair.second.isAvailable) {
            availableDrivers.push_back(pair.second);
        }
    }

    if (availableDrivers.empty()) {
        return {false, "No available drivers at the moment. Please try again later.", "", "", "", 0.0, 0.0, {}};
    }

    // 3. Find nearest driver using Matching Engine
    double distanceToRider = 0.0;
    std::string nearestDriverId = MatchingEngine::findNearestDriver(pickupLocation, availableDrivers, mapGraph, distanceToRider);

    if (nearestDriverId.empty()) {
        return {false, "No available drivers can reach your location '" + pickupLocation + "'.", "", "", "", 0.0, 0.0, {}};
    }

    // 4. Calculate Shortest Path distance from Pickup to Destination for Fare calculation
    auto distancesFromPickup = mapGraph.dijkstra(pickupLocation);
    double rideDistance = distancesFromPickup[destination];
    double fare = calculateFare(rideDistance);

    // 5. Assign Driver & Handle Availability
    drivers[nearestDriverId].isAvailable = false;
    
    std::string rideId = generateRideId();
    Ride newRide = {rideId, nearestDriverId, pickupLocation, destination, rideDistance, fare};
    rideHistory.push_back(newRide);

    // Update driver location (Simulating immediate completion for project simplicity)
    drivers[nearestDriverId].currentLocation = destination;
    drivers[nearestDriverId].isAvailable = true;

    auto path = mapGraph.getShortestPath(pickupLocation, destination);

    return {true, "Success", rideId, nearestDriverId, drivers[nearestDriverId].name, rideDistance, fare, path};
}

std::vector<Driver> RideManager::getAllDrivers() const {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<Driver> driverList;
    for (const auto& pair : drivers) driverList.push_back(pair.second);
    return driverList;
}

std::vector<Ride> RideManager::getRideHistory() const {
    std::lock_guard<std::mutex> lock(systemMutex);
    return rideHistory;
}

void RideManager::updateDriverLocation(const std::string& driverId, const std::string& newLocation) {
    std::lock_guard<std::mutex> lock(systemMutex);
    if (drivers.find(driverId) != drivers.end()) {
        drivers[driverId].currentLocation = newLocation;
    }
}

double RideManager::calculateFare(double distance) {
    return BASE_FARE + (distance * COST_PER_KM);
}

std::string RideManager::generateRideId() {
    return "RIDE" + std::to_string(rideHistory.size() + 1);
}
