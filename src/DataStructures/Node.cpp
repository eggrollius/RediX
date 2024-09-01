#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
private:
  T value;
public:
  Node<T>* next;
  Node<T>* previous;

  Node(const T& value);

  T get_value() const;
  void set_value(const T& value);
};

template <typename T>
Node<T>::Node(const T& value) : value(value), next(nullptr) {}

template <typename T>
T Node<T>::get_value() const {
  return this->value;
}

template <typename T>
void Node<T>::set_value(const T& value) {
  this->value = value;
}

#endif // NODE_H