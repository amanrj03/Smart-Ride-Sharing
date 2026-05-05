#pragma once

#include <string>

struct Location {
    std::string name; // e.g. "A", "B", "C"
};

struct Driver {
    std::string driverId;
    std::string name;
    std::string currentLocation;
    bool isAvailable;
};



struct Ride {
    std::string rideId;
    std::string driverId;
    std::string startLocation;
    std::string endLocation;
    double distance;
    double fare;
};
