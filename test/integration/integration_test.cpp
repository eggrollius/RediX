#include <iostream>
#include <thread>
#include "server.h"
#include "client.h"

// Helper function to perform the test
void run_integration_test() {
    // Initialize server and client
    Server server(1234);
    Client client;
    if(!server.start()) {
      std::cout << "Cannot start server" << std::endl;
      return;
    } else {
      std::cout << "Server started" << std::endl;
    }

    // Start the server in a separate thread
    std::thread server_thread([&server]() {
        server.accept_client();
        server.run();
    });

    // Set up the client
    if(!client.connect_to_server(1234)) {
      std::cout << "could not connect to server" << std::endl;
      return;
    } else {
      std::cout << "Client connected to server" << std::endl;
    }

    // Example test: check if the client can send a request and receive a valid response
    std::string request = "SET key1 val1";
    std::string expected_response = "Set operation successful";
    
    if(!client.send_request(request)) {
      std::cout << "could not send message to server" << std::endl;
    } else {
      std::cout << "Sent message to server" << std::endl;
    }
    
    std::string response = client.read_response();
    
    if (response == expected_response) {
        std::cout << "Test passed: Received expected response" << std::endl;
    } else {
        std::cout << "Test failed: Expected \"" << expected_response 
                  << "\" but got \"" << response << "\"" << std::endl;
    }

    // Stop the server after the test
    server.stop();
    server_thread.join(); // Ensure server thread completes
}

int main() {
    run_integration_test();
    return 0;
}
