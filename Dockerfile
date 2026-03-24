# --- Build Stage ---
FROM gcc:12 AS build

# Install CMake
RUN apt-get update && apt-get install -y cmake

# Set working directory
WORKDIR /app

# Copy source code
COPY . /app

# Create build directory
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build . --config Release

# --- Final Stage ---
FROM debian:bullseye-slim

WORKDIR /app

# Copy the executable from build stage
COPY --from=build /app/build/infradb_core_app /app/infradb_core_app

# Expose the default Render port
EXPOSE 8080

# Run the app
CMD ["./infradb_core_app"]
