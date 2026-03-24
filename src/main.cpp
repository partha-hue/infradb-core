#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "infradb/core/Engine.hpp"

// Minimal HTTP Server using simple socket for health check on Render
// For production, consider using Crow or Drogon
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

void run_health_check_server(int port) {
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    #ifndef _WIN32
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    #endif

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed to port " << port << std::endl;
        return;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "InfraDB Core Health Server running on port " << port << std::endl;

    while (true) {
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket >= 0) {
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\nOK";
            #ifdef _WIN32
                send(new_socket, response.c_str(), (int)response.length(), 0);
                closesocket(new_socket);
            #else
                send(new_socket, response.c_str(), response.length(), 0);
                close(new_socket);
            #endif
        }
    }
}

int main() {
    std::cout << "Starting InfraDB Core Engine..." << std::endl;
    
    // Initialize Engine
    infradb::core::Engine engine;
    
    // Get port from environment or default to 8080
    const char* port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 8080;

    // Start health check server in a separate thread
    std::thread health_thread(run_health_check_server, port);
    
    std::cout << "InfraDB Core is active and monitoring." << std::endl;
    
    // Keep main thread alive
    health_thread.join();
    
    return 0;
}
