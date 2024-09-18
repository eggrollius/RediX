#include <csignal>
#include <iostream>
#include "server.h"

Server* server_ptr = nullptr;
bool interupted = false;
void handle_signal(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nSIGINT received, shutting down the server..." << std::endl;
        if (server_ptr) {
            server_ptr->stop();  // Gracefully stop the server
        }
        interupted = true;
    }
}

int main(int argc, char* argv[]) {
    
    try {
        std::string addr = "127.0.0.1";
        int port = 6379;

        if (argc >= 2) {
            addr = argv[1];
        }
        if (argc >= 3) {
            port = std::stoi(argv[2]);
        }

        Server server(addr, port);
        server_ptr = &server;

        // Register signal handler for Ctrl+C (SIGINT)
        std::signal(SIGINT, handle_signal);

        if (server.start()) {
            std::cout << "Server running at " << addr << ":" << port << ". Press Ctrl+C to stop." << std::endl;
            while(!interupted) {}
        } else {
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
        return 1;
    }

    return 0;
}
