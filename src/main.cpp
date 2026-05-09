#include "httplib.h"
#include "nlohmann/json.hpp"
#include "RideManager.h"
#include <cstdlib>
#include <string>

using json = nlohmann::json;

static const std::string CORS_ORIGIN = "https://smart-ride-frontend.onrender.com";

void addCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", CORS_ORIGIN);
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Accept");
    res.set_header("Access-Control-Max-Age", "86400");
}

int main() {
    httplib::Server app;

    RideManager system;

    system.addLocationEdge("A", "B", 3.0);
    system.addLocationEdge("A", "E", 4.0);
    system.addLocationEdge("B", "C", 3.0);
    system.addLocationEdge("B", "F", 4.0);
    system.addLocationEdge("B", "E", 5.0);
    system.addLocationEdge("C", "D", 3.0);
    system.addLocationEdge("C", "G", 4.0);
    system.addLocationEdge("C", "J", 8.0);
    system.addLocationEdge("D", "G", 5.0);
    system.addLocationEdge("E", "F", 3.0);
    system.addLocationEdge("E", "H", 4.0);
    system.addLocationEdge("E", "I", 6.0);
    system.addLocationEdge("F", "I", 4.0);
    system.addLocationEdge("F", "G", 3.0);
    system.addLocationEdge("G", "J", 5.0);
    system.addLocationEdge("H", "I", 3.0);
    system.addLocationEdge("I", "J", 4.0);

    system.addDriver({"D1", "Rakesh", "A", true});
    system.addDriver({"D2", "Sunil", "D", true});
    system.addDriver({"D3", "Karan", "F", true});
    system.addDriver({"D4", "Varun", "H", true});
    system.addDriver({"D5", "Amit", "J", true});

    app.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Smart Ride-Sharing Backend is running.", "text/plain");
    });

    // OPTIONS preflight handlers
    app.Options("/api/drivers", [](const httplib::Request&, httplib::Response& res) {
        addCors(res);
        res.status = 200;
    });

    app.Options("/api/history", [](const httplib::Request&, httplib::Response& res) {
        addCors(res);
        res.status = 200;
    });

    app.Options("/api/request_ride", [](const httplib::Request&, httplib::Response& res) {
        addCors(res);
        res.status = 200;
    });

    app.Get("/api/drivers", [&system](const httplib::Request&, httplib::Response& res) {
        auto drivers = system.getAllDrivers();
        json driverList = json::array();
        for (const auto& d : drivers) {
            driverList.push_back({
                {"id", d.driverId},
                {"name", d.name},
                {"location", d.currentLocation},
                {"available", d.isAvailable}
            });
        }
        json body = {{"drivers", driverList}};
        addCors(res);
        res.set_content(body.dump(), "application/json");
    });

    app.Get("/api/history", [&system](const httplib::Request&, httplib::Response& res) {
        auto history = system.getRideHistory();
        json historyList = json::array();
        for (const auto& r : history) {
            historyList.push_back({
                {"rideId", r.rideId},
                {"driverId", r.driverId},
                {"pickup", r.startLocation},
                {"destination", r.endLocation},
                {"distance", r.distance},
                {"fare", r.fare}
            });
        }
        json body = {{"history", historyList}};
        addCors(res);
        res.set_content(body.dump(), "application/json");
    });

    app.Post("/api/request_ride", [&system](const httplib::Request& req, httplib::Response& res) {
        addCors(res);
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid JSON\"}", "application/json");
            return;
        }

        std::string pickup = body["pickup"];
        std::string destination = body["destination"];
        RideResult result = system.processRideRequest(pickup, destination);

        json resBody;
        if (result.success) {
            resBody["status"] = "success";
            resBody["message"] = result.message;
            resBody["rideId"] = result.rideId;
            resBody["driverId"] = result.driverId;
            resBody["driverName"] = result.driverName;
            resBody["distance"] = result.distance;
            resBody["fare"] = result.fare;
            resBody["path"] = result.path;
        } else {
            resBody["status"] = "error";
            resBody["message"] = result.message;
        }
        res.set_content(resBody.dump(), "application/json");
    });

    int port = 9090;
    if (const char* env_p = std::getenv("PORT")) {
        port = std::stoi(env_p);
    }

    app.listen("0.0.0.0", port);
}
