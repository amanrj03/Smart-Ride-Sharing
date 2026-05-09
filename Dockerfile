# Build stage
FROM ubuntu:22.04 AS build

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy CMakeLists.txt and source code
COPY CMakeLists.txt .
COPY src/ ./src/

# Build the application
RUN mkdir build && cd build && cmake .. && make

# Runtime stage
FROM ubuntu:22.04

# Install only necessary runtime libraries if any
RUN apt-get update && apt-get install -y \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the compiled executable from the build stage
COPY --from=build /app/build/ride_sharing .

# Render dynamically sets the PORT environment variable
ENV PORT=9090
EXPOSE ${PORT}

# Run the backend
CMD ["./ride_sharing"]
