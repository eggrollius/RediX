#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

// Node class definition
template <typename T>
class Node {
public:
    T value;
    Node* next;
    Node* previous;

    Node(const T& value) : value(value), next(nullptr), previous(nullptr) {}
    T get_value() const { return value; }
};

// LinkedList class definition
template <typename T>
class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    
    bool is_empty() const;
    int length() const;
    
    void left_push(const T& value);
    void right_push(const T& value);
    
    T left_pop();
    T right_pop();

private:
    int size;
    Node<T>* head;
    Node<T>* tail;
};

// Implementation

template <typename T>
LinkedList<T>::LinkedList() : size(0), head(nullptr), tail(nullptr) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node<T>* current = this->head;
    while (current != nullptr) {
        Node<T>* next = current->next;
        delete current;
        current = next;
    }
}

template <typename T>
bool LinkedList<T>::is_empty() const {
    return this->size == 0;
}

template <typename T>
int LinkedList<T>::length() const {
    return this->size;
}

template <typename T>
void LinkedList<T>::left_push(const T& value) {
    Node<T>* new_head = new Node<T>(value);
    new_head->next = this->head;

    if (this->head != nullptr) {
        this->head->previous = new_head;
    }
    this->head = new_head;

    if (this->tail == nullptr) {
        this->tail = new_head;
    }

    this->size += 1;
}

template <typename T>
void LinkedList<T>::right_push(const T& value) {
    Node<T>* new_tail = new Node<T>(value);
    if (this->tail != nullptr) {
        this->tail->next = new_tail;
        new_tail->previous = this->tail;
    }
    this->tail = new_tail;

    if (this->head == nullptr) {
        this->head = new_tail;
    }

    this->size += 1;
}

template <typename T>
T LinkedList<T>::left_pop() {
    if (this->head == nullptr) {
        throw std::out_of_range("List is empty");
    }

    T return_value = this->head->get_value();
    Node<T>* new_head = this->head->next;

    delete this->head;

    this->head = new_head;

    if (this->head != nullptr) {
        this->head->previous = nullptr;
    } else {
        this->tail = nullptr;
    }

    this->size -= 1;

    return return_value;
}

template <typename T>
T LinkedList<T>::right_pop() {
    if (this->tail == nullptr) {
        throw std::out_of_range("List is empty");
    }

    T return_value = this->tail->get_value();
    Node<T>* new_tail = this->tail->previous;

    delete this->tail;

    this->tail = new_tail;

    if (this->tail != nullptr) {
        this->tail->next = nullptr;
    } else {
        this->head = nullptr;
    }

    this->size -= 1;

    return return_value;
}

#endif // LINKEDLIST_H
