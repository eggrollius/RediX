#ifndef NODE_H
#define NODE_H

#include <string>
class Node {
private:
  std::string value;
public:
  Node* next;
  Node* previous;

  Node(const std::string& value);

  std::string get_value() const;
  void set_value(const std::string& value);
};

#endif // NODE_H