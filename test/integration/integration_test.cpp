#include "ResponseMessage.h"
#include <iostream>
#include "server.h"
#include "client.h"

// Helper function to perform the test
void run_integration_test() {
  // Initialize server and client
  Server server(1234);
  Client client(1234);

  // Start the server
  server.start();


  // Set up the client
  if (!client.connect_to_server()) {
    std::cout << "Could not connect to server" << std::endl;
    return;
  }
  std::cout << "Client connected to server" << std::endl;

  // test: check if the client can send a request and receive a valid response
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

  server.stop(); 
}

int main() {
    run_integration_test();
    return 0;
}
