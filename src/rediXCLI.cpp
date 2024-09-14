#include "OperationTypes.h"
#include "client.h"
#include <iostream>

void parse_cmd_from_str(const std::string &cmd_str, std::vector<std::string> &cmd);

int main(int argc, char* argv[]) {
    std::string server_ip = "127.0.0.1";
    int port = 6379;
    if(argc >=2) {
        // server ip
        server_ip = argv[1];
    }
    if(argc >= 3) {
        port = std::stoi(argv[2]);
    }
    
    // Create a client and connect
    Client client(server_ip, port);
    if(!client.connect_to_server()) {
        std::cerr << "Cannot connect to server at: " << server_ip << ":" << port << std::endl;
        return 1;
    }

    bool exit = false;
    while(!exit){
        std::cout << server_ip << ":" << port << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> cmd;
        parse_cmd_from_str(input, cmd);
        if(cmd.size() <= 0) {
            continue;
        }

        std::string operation = cmd[0];
        std::string res_msg;
        if(cmd[0] == "exit") {
            exit = true;
        } else if (operation_is(operation, Operation::GET) && cmd.size() == 2) {
            res_msg = client.get(cmd[1]);
        } else if (operation_is(operation, Operation::SET) && cmd.size() == 3) {
            res_msg = client.set(cmd[1], cmd[2]);
        } else if (operation_is(operation, Operation::DEL) && cmd.size() == 2) {
            res_msg = client.del(cmd[1]);
        } else if (operation_is(operation, Operation::EXPIRE) && cmd.size() == 3) {
            res_msg = client.expire(cmd[1], std::stoi(cmd[2]));
        } else if (operation_is(operation, Operation::TTL) && cmd.size() == 2) {
            res_msg = std::to_string(client.ttl(cmd[1]));
        } else if (operation_is(operation, Operation::LPUSH) && cmd.size() == 3) {
            res_msg = client.lpush(cmd[1], cmd[2]);
        } else if (operation_is(operation, Operation::RPUSH) && cmd.size() == 3) {
            res_msg = client.rpush(cmd[1], cmd[2]);
        } else if (operation_is(operation, Operation::LPOP) && cmd.size() == 2) {
            res_msg = client.lpop(cmd[1]);
        } else if (operation_is(operation, Operation::RPOP) && cmd.size() == 2) {
            res_msg = client.rpop(cmd[1]);
        } else if (operation_is(operation, Operation::LLEN) && cmd.size() == 2) {
            res_msg = client.llen(cmd[1]);
        } else if(operation_is(operation, Operation::ZADD) && cmd.size() == 4){
            res_msg = client.ZAdd(cmd[1], cmd[2], std::stod(cmd[3]));
        } else if(operation_is(operation, Operation::ZREM) && cmd.size() == 3) {
            res_msg = client.ZRem(cmd[1], cmd[2]);
        } else if(operation_is(operation, Operation::ZRANGEBYSCORE) && cmd.size() == 4) {
            res_msg = client.ZRangeByScore(cmd[1], std::stod(cmd[2]), std::stod(cmd[3]));
        } else {
            res_msg = "Unknown operation \"" + cmd[1] +"\"";
        }

        std::cout << res_msg << std::endl;
    }

    return 0;
}

void parse_cmd_from_str(const std::string &cmd_str, std::vector<std::string> &cmd) {
    std::istringstream stream(cmd_str);
    std::string token;

    while (stream >> token) { // Extract tokens separated by whitespace
      cmd.push_back(token); // Add the token to the output vector
    }

    return; 
}