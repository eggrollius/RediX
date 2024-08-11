#include <iostream>
#include <cassert>
#include "../../server/database.h"
#include "../../libshared/libshared.h"

void print_test_result(const std::string &test_name, bool result) {
    std::cout << (result ? "PASSED" : "FAILED") << ": " << test_name << std::endl;
}

void test_set_value() {
    Database db;
    std::string res_msg;
    Db_return_code result = db.set_value("test_key", "test_value", res_msg);

    bool success = (result == RES_OK && res_msg == "Set operation successful");
    print_test_result("test_set_value", success);
}

void test_get_value() {
    Database db;
    std::string res_msg;
    db.set_value("test_key", "test_value", res_msg);

    std::string value;
    Db_return_code result = db.get_value("test_key", value);

    bool success = (result == RES_OK && value == "test_value");
    print_test_result("test_get_value", success);
}

void test_get_value_not_found() {
    Database db;
    std::string value;
    std::string res_msg;
    Db_return_code result = db.get_value("nonexistent_key", value);

    bool success = (result == RES_NX);
    print_test_result("test_get_value_not_found", success);
}

void test_del_entry() {
    Database db;
    std::string res_msg;
    db.set_value("test_key", "test_value", res_msg);

    Db_return_code result = db.del_entry("test_key", res_msg);

    bool success = (result == RES_OK);

    // Verify the entry is deleted
    std::string value;
    result = db.get_value("test_key", value);
    success = success && (result == RES_NX);
    print_test_result("test_del_entry", success);
}

void test_set_ttl() {
    Database db;
    std::string res_msg;
    Db_return_code result = db.set_ttl("test_key", 60, res_msg);

    bool success = (result == RES_OK);
    print_test_result("test_set_ttl", success);
}

void test_get_ttl() {
    Database db;
    std::string set_res_msg;
    db.set_ttl("test_key", 60, set_res_msg);

    std::string get_res_msg;
    Db_return_code result = db.get_ttl("test_key", get_res_msg);

    bool success = (result == RES_OK && get_res_msg == "60");
    print_test_result("test_get_ttl", success);
}

void test_get_ttl_not_found() {
    Database db;
    std::string res_msg;
    Db_return_code result = db.get_ttl("nonexistent_key", res_msg);

    bool success = (result == RES_NX);
    print_test_result("test_get_ttl_not_found", success);
}

int main() {
    test_set_value();
    test_get_value();
    test_get_value_not_found();
    test_del_entry();
    test_set_ttl();
    test_get_ttl();
    test_get_ttl_not_found();

    return 0;
}
