cmake_minimum_required(VERSION 3.14)
project(SmartRideSharingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# FetchContent for Crow and Asio
include(FetchContent)

# ASIO (required by Crow)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        asio-1-28-0
)
FetchContent_MakeAvailable(asio)
FetchContent_GetProperties(asio)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include CACHE PATH "" FORCE)
include_directories(${ASIO_INCLUDE_DIR})
add_definitions(-DASIO_STANDALONE)

# CROW
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG        v1.2.0
)
FetchContent_MakeAvailable(crow)
FetchContent_GetProperties(crow)
include_directories(${crow_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Graph.cpp
    src/MatchingEngine.cpp
    src/RideManager.cpp
)

add_executable(ride_sharing ${SOURCES})

# Link libraries (Crow is header-only, but requires Windows socket libraries)
if(WIN32)
    target_link_libraries(ride_sharing ws2_32 mswsock)
endif()
