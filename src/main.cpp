#include "crow.h"
#include "crow/middlewares/cors.h"
#include "RideManager.h"
#include <cstdlib>

int main() {
    crow::App<crow::CORSHandler> app;

    // Configure CORS globally
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .headers("*")
        .methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get, crow::HTTPMethod::Options)
        .origin("*");

    RideManager system;

    // Initialize Map Data (Locations and edges) to match frontend
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

    // Initialize some drivers spread across the new grid
    system.addDriver({"D1", "Rakesh", "A", true});
    system.addDriver({"D2", "Sunil", "D", true});
    system.addDriver({"D3", "Karan", "F", true});
    system.addDriver({"D4", "Varun", "H", true});
    system.addDriver({"D5", "Amit", "J", true});


    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Get)([]() {
        return "Smart Ride-Sharing Backend is running. Please visit the frontend URL to use the application.";
    });

    CROW_ROUTE(app, "/api/drivers").methods(crow::HTTPMethod::Get)([&system]() {
        auto drivers = system.getAllDrivers();
        std::vector<crow::json::wvalue> driverList;
        for (const auto& d : drivers) {
            crow::json::wvalue driverJson;
            driverJson["id"] = d.driverId;
            driverJson["name"] = d.name;
            driverJson["location"] = d.currentLocation;
            driverJson["available"] = d.isAvailable;
            driverList.push_back(driverJson);
        }
        crow::json::wvalue res;
        res["drivers"] = crow::json::wvalue::list(driverList);
        
        auto response = crow::response(res);
        return response;
    });

    CROW_ROUTE(app, "/api/history").methods(crow::HTTPMethod::Get)([&system]() {
        auto history = system.getRideHistory();
        std::vector<crow::json::wvalue> historyList;
        for (const auto& r : history) {
            crow::json::wvalue rideJson;
            rideJson["rideId"] = r.rideId;
            rideJson["driverId"] = r.driverId;
            rideJson["pickup"] = r.startLocation;
            rideJson["destination"] = r.endLocation;
            rideJson["distance"] = r.distance;
            rideJson["fare"] = r.fare;
            historyList.push_back(rideJson);
        }
        crow::json::wvalue res;
        res["history"] = crow::json::wvalue::list(historyList);
        
        auto response = crow::response(res);
        return response;
    });

    CROW_ROUTE(app, "/api/request_ride").methods(crow::HTTPMethod::Post, crow::HTTPMethod::Options)([&system](const crow::request& req) {
        if (req.method == crow::HTTPMethod::Options) {
            auto res = crow::response(204);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            return res;
        }

        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string pickup = body["pickup"].s();
        std::string destination = body["destination"].s();

        RideResult result = system.processRideRequest(pickup, destination);

        crow::json::wvalue res;
        if (result.success) {
            res["status"] = "success";
            res["message"] = result.message;
            res["rideId"] = result.rideId;
            res["driverId"] = result.driverId;
            res["driverName"] = result.driverName;
            res["distance"] = result.distance;
            res["fare"] = result.fare;
            
            std::vector<crow::json::wvalue> pathList;
            for (const auto& node : result.path) {
                pathList.push_back(crow::json::wvalue(node));
            }
            res["path"] = crow::json::wvalue::list(pathList);
        } else {
            res["status"] = "error";
            res["message"] = result.message;
        }

        auto response = crow::response(res);
        response.add_header("Access-Control-Allow-Origin", "*");
        return response;
    });

    int port = 9090;
    if (const char* env_p = std::getenv("PORT")) {
        port = std::stoi(env_p);
    }
    app.port(port).multithreaded().run();
}
