#include "crow.h"
#include "RideManager.h"
#include <cstdlib>

// Adds CORS headers to every response
void addCorsHeaders(crow::response& res) {
    res.add_header("Access-Control-Allow-Origin", "https://smart-ride-frontend.onrender.com");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type, Accept");
    res.add_header("Access-Control-Max-Age", "86400");
}

// Returns a 200 preflight response with CORS headers
crow::response preflightResponse() {
    crow::response res(200);
    addCorsHeaders(res);
    return res;
}

int main() {
    crow::SimpleApp app;

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

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Get)([]() {
        return "Smart Ride-Sharing Backend is running.";
    });

    // OPTIONS preflight handlers
    CROW_ROUTE(app, "/api/drivers").methods(crow::HTTPMethod::Options)([]() {
        return preflightResponse();
    });

    CROW_ROUTE(app, "/api/history").methods(crow::HTTPMethod::Options)([]() {
        return preflightResponse();
    });

    CROW_ROUTE(app, "/api/request_ride").methods(crow::HTTPMethod::Options)([]() {
        return preflightResponse();
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
        crow::json::wvalue body;
        body["drivers"] = crow::json::wvalue::list(driverList);
        auto res = crow::response(body);
        addCorsHeaders(res);
        return res;
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
        crow::json::wvalue body;
        body["history"] = crow::json::wvalue::list(historyList);
        auto res = crow::response(body);
        addCorsHeaders(res);
        return res;
    });

    CROW_ROUTE(app, "/api/request_ride").methods(crow::HTTPMethod::Post)([&system](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            auto res = crow::response(400, "Invalid JSON");
            addCorsHeaders(res);
            return res;
        }

        std::string pickup = body["pickup"].s();
        std::string destination = body["destination"].s();

        RideResult result = system.processRideRequest(pickup, destination);

        crow::json::wvalue resBody;
        if (result.success) {
            resBody["status"] = "success";
            resBody["message"] = result.message;
            resBody["rideId"] = result.rideId;
            resBody["driverId"] = result.driverId;
            resBody["driverName"] = result.driverName;
            resBody["distance"] = result.distance;
            resBody["fare"] = result.fare;
            std::vector<crow::json::wvalue> pathList;
            for (const auto& node : result.path) {
                pathList.push_back(crow::json::wvalue(node));
            }
            resBody["path"] = crow::json::wvalue::list(pathList);
        } else {
            resBody["status"] = "error";
            resBody["message"] = result.message;
        }

        auto res = crow::response(resBody);
        addCorsHeaders(res);
        return res;
    });

    int port = 9090;
    if (const char* env_p = std::getenv("PORT")) {
        port = std::stoi(env_p);
    }
    app.bindaddr("0.0.0.0").port(port).multithreaded().run();
}
