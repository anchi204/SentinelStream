FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y build-essential cmake pkg-config librdkafka-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN cmake -S . -B build -DSENTINELSTREAM_BUILD_TESTS=OFF && cmake --build build --parallel
FROM ubuntu:24.04
RUN apt-get update && apt-get install -y librdkafka1 && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /app/build/sentinelstream /app/sentinelstream
CMD ["/app/sentinelstream"]
