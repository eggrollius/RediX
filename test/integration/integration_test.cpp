#include "ResponseMessage.h"
#include <iostream>
#include <thread>
#include "server.h"
#include "client.h"

// Helper function to perform the test
void run_integration_test() {
  // Initialize server and client
  Server server(1234);
  Client client;

  // Start the server in a separate thread
  std::thread server_thread([&server]() {
    if (!server.start()) {
        std::cout << "Cannot start server" << std::endl;
        return;
    }
    std::cout << "Server started" << std::endl;
    while(true) {
      server.accept_client();
      server.run();
    }
  });

  // Set up the client in a separate thread
  std::thread client_thread([&client]() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Ensure server is up
    if (!client.connect_to_server(1234)) {
      std::cout << "Could not connect to server" << std::endl;
      return;
    }
    std::cout << "Client connected to server" << std::endl;

    // Example test: check if the client can send a request and receive a valid response
    std::string request = "SET key1 val1";
    std::string expected_response = Response::ToString(ResponseMessage::OK);

    if (!client.send_request(request)) {
      std::cout << "Could not send message to server" << std::endl;
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
  });

  // Wait for both threads to complete
  client_thread.join();
  server.stop(); // Stop the server
  server_thread.join(); // Ensure server thread completes
}

int main() {
    run_integration_test();
    return 0;
}
