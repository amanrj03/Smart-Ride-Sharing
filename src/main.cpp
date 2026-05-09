#include "crow.h"
#include "RideManager.h"
#include <cstdlib>

static const std::string CORS_ORIGIN = "https://smart-ride-frontend.onrender.com";

void addCors(crow::response& res) {
    res.add_header("Access-Control-Allow-Origin", CORS_ORIGIN);
    res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type, Accept");
    res.add_header("Access-Control-Max-Age", "86400");
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

    CROW_ROUTE(app, "/").methods("GET"_method)([]() {
        return "Smart Ride-Sharing Backend is running.";
    });

    CROW_ROUTE(app, "/api/drivers").methods("GET"_method, "OPTIONS"_method)
    ([&system](const crow::request& req) {
        crow::response res;
        addCors(res);
        if (req.method == "OPTIONS"_method) {
            res.code = 200;
            return res;
        }
        auto drivers = system.getAllDrivers();
        std::vector<crow::json::wvalue> driverList;
        for (const auto& d : drivers) {
            crow::json::wvalue dj;
            dj["id"] = d.driverId;
            dj["name"] = d.name;
            dj["location"] = d.currentLocation;
            dj["available"] = d.isAvailable;
            driverList.push_back(std::move(dj));
        }
        crow::json::wvalue body;
        body["drivers"] = crow::json::wvalue::list(driverList);
        res.write(body.dump());
        res.add_header("Content-Type", "application/json");
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/history").methods("GET"_method, "OPTIONS"_method)
    ([&system](const crow::request& req) {
        crow::response res;
        addCors(res);
        if (req.method == "OPTIONS"_method) {
            res.code = 200;
            return res;
        }
        auto history = system.getRideHistory();
        std::vector<crow::json::wvalue> historyList;
        for (const auto& r : history) {
            crow::json::wvalue rj;
            rj["rideId"] = r.rideId;
            rj["driverId"] = r.driverId;
            rj["pickup"] = r.startLocation;
            rj["destination"] = r.endLocation;
            rj["distance"] = r.distance;
            rj["fare"] = r.fare;
            historyList.push_back(std::move(rj));
        }
        crow::json::wvalue body;
        body["history"] = crow::json::wvalue::list(historyList);
        res.write(body.dump());
        res.add_header("Content-Type", "application/json");
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/request_ride").methods("POST"_method, "OPTIONS"_method)
    ([&system](const crow::request& req) {
        crow::response res;
        addCors(res);
        if (req.method == "OPTIONS"_method) {
            res.code = 200;
            return res;
        }
        auto body = crow::json::load(req.body);
        if (!body) {
            res.code = 400;
            res.write("Invalid JSON");
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
            for (const auto& node : result.path)
                pathList.push_back(crow::json::wvalue(node));
            resBody["path"] = crow::json::wvalue::list(pathList);
        } else {
            resBody["status"] = "error";
            resBody["message"] = result.message;
        }
        res.write(resBody.dump());
        res.add_header("Content-Type", "application/json");
        res.code = 200;
        return res;
    });

    int port = 9090;
    if (const char* env_p = std::getenv("PORT")) {
        port = std::stoi(env_p);
    }
    app.bindaddr("0.0.0.0").port(port).multithreaded().run();
}
