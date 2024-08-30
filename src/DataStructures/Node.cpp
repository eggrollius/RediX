#include "Node.h"
#include <string>

Node::Node(const std::string& value) : value(value), next(nullptr) {}

std::string Node::get_value() const {
  return this->value;
}

void Node::set_value(const std::string& value) {
  this->value = value;
}