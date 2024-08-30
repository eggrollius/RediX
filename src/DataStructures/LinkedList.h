#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"

class LinkedList {
private:
  int size;
  Node* head;
  Node* tail;
public:
  LinkedList();
  ~LinkedList();
  int length() const;
  void left_push(const std::string& value);
  void right_push(const std::string& value);
  std::string left_pop();
  std::string right_pop();
};

#endif // LINKEDLIST_H