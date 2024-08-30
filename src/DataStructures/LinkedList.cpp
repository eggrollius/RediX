#include "LinkedList.h"

#include <stdexcept>

LinkedList::LinkedList() : size(0), head(nullptr), tail(nullptr) {}

LinkedList::~LinkedList() {
  Node* current = this->head;
  while(current != nullptr) {
    Node* next = current->next;
    delete current;
    current = next;
  }
}

int LinkedList::length() const {
  return this->size;
}

void LinkedList::left_push(const std::string& value) {
  Node* new_head = new Node(value);
  new_head->next = this->head;

  this->head = new_head;

  this->size += 1;
}

void LinkedList::right_push(const std::string& value) {
  Node* new_tail = new Node(value);
  this->tail->next = new_tail;

  this->tail = new_tail;

  this->size += 1;
}

std::string LinkedList::left_pop() {
  if (this->head == nullptr) {
    throw std::runtime_error("LinekdList is empty, cannot left_pop()");
  }

  std::string return_value = this->head->get_value();
  Node* new_head = this->head->next;

  delete this->head;

  this->head = new_head;

  if(this->head != nullptr) {
    this->head->previous = nullptr;
  }
  
  this->size -= 1;

  return return_value;
}

std::string LinkedList::right_pop() {
  if (this->tail == nullptr) {
    throw std::runtime_error("LinekdList is empty, cannot right_pop()");
  }

  std::string return_value = this->tail->get_value();
  Node* new_tail = this->tail->previous;

  delete this->tail;

  this->tail = new_tail;

  if(this->tail != nullptr) {
    this->tail->next = nullptr;
  }

  this->size -= 1;

  return return_value;
}