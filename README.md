# Smart Ride-Sharing 🚗

A full-stack, industry-grade ride-sharing application built to demonstrate real-time driver matching using shortest-path graph algorithms.

## 🌟 Features

- **Algorithmic Driver Matching**: Uses **Dijkstra's Algorithm** to calculate the absolute shortest path between a rider and the nearest available driver.
- **Dynamic Live Map**: A beautiful, interactive React frontend that visualizes the city graph (nodes, edges, and distances) using clean SVG rendering and quadratic bezier curves.
- **Real-Time State Management**: Tracks driver availability and locations. Drivers automatically move to the destination node upon completing a ride.
- **RESTful API**: Fast and concurrent backend served by the C++ Crow framework.

## 🛠️ Technology Stack

- **Backend**: C++ (C++17/20), [Crow](https://crowcpp.org/) (Fast microframework), CMake
- **Frontend**: React.js, Vite, Vanilla CSS (Glassmorphism & dynamic styling)
- **Algorithms**: Priority Queue Min-Heap, Dijkstra's Shortest Path

## 🚀 Getting Started

### Prerequisites
- Node.js (v16+)
- CMake (3.15+)
- A C++ Compiler (GCC, MSVC, or Clang)

### 1. Build and Run the Backend (C++)
The backend relies on CMake to download dependencies (Crow and Asio) and build the executable.

```bash
# Navigate to the project root
mkdir -p build
cd build

# Configure CMake
cmake ..

# Compile the backend
cmake --build .

# Run the server (starts on http://localhost:9090)
./ride_sharing.exe  # On Windows
# ./ride_sharing    # On macOS/Linux
```

### 2. Run the Frontend (React)
Open a new terminal window to start the frontend Vite development server.

```bash
# Navigate to the frontend directory
cd frontend

# Install dependencies (only needed the first time)
npm install

# Start the dev server
npm run dev
```
Open your browser to the local URL provided by Vite (usually `http://localhost:5173`) to see the application!

## 📸 How It Works
1. The backend initializes a hardcoded city grid consisting of connected nodes (A, B, C... J) with varying distances.
2. Drivers are spawned at various nodes across the map.
3. Using the frontend UI, select a **Pickup Node** and a **Destination Node**.
4. The backend computes the shortest paths and assigns the nearest available driver to your pickup location.
5. Upon confirmation, you receive a full breakdown of the route path, distance, and fare (in Rs.). The driver's location is then updated on the live map.

## 📜 License
This project is for personal and educational purposes. Feel free to use the code as inspiration!
