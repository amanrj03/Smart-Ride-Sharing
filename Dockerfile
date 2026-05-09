# Build stage
FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .

# Copy source - any change here invalidates the build cache
COPY src/ ./src/
COPY .buildversion ./

RUN mkdir build && cd build && cmake .. && make

# Runtime stage
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    ca-certificates \
    libssl3 \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /app/build/ride_sharing .

ENV PORT=9090
EXPOSE ${PORT}

CMD ["./ride_sharing"]
