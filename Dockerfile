# --- Build Stage ---
# Use Debian-based image for better GLIBC compatibility
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
# Match the GLIBC version by using a newer Debian base
FROM debian:bookworm-slim

WORKDIR /app

# Copy the executable from build stage
COPY --from=build /app/build/infradb_core_app /app/infradb_core_app

# Ensure we have the necessary runtime libraries
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Expose the default Render port
EXPOSE 8080

# Run the app
CMD ["./infradb_core_app"]
