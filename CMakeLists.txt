cmake_minimum_required(VERSION 3.14)
project(SmartRideSharingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/src)

include(FetchContent)

# cpp-httplib (header-only, full OPTIONS support)
FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG        v0.18.5
)
FetchContent_MakeAvailable(httplib)

# nlohmann/json for JSON handling
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

set(SOURCES
    src/main.cpp
    src/Graph.cpp
    src/MatchingEngine.cpp
    src/RideManager.cpp
)

add_executable(ride_sharing ${SOURCES})

target_link_libraries(ride_sharing httplib::httplib nlohmann_json::nlohmann_json)

if(WIN32)
    target_link_libraries(ride_sharing ws2_32 mswsock)
endif()
