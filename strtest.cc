#include <iostream>
#include <cstring>
#include <string>

int main() {
    char buf[1000];
    std::string testString = "TestString";
    int len = testString.length();

    memcpy(&buf[0], &testString, len);

    std::cout << buf << std::endl;
}