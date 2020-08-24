//
// Created by vincent on 24.08.20.
//

#include "Array.h"

#include <stdio.h>
#include <stdlib.h>

template <typename T>
Array<T>::Array(unsigned int size) {
  this->items = (T*)malloc(sizeof(T) * size);
  this->size = size;
  this->count = 0;
};

template <typename T>
Array<T>::~Array() {
  free(this->items);
}

template <typename T>
void Array<T>::Add(T item) {
  if (size == count) {
    this->items = (T*)realloc(this->items, sizeof(T) * (size + 10));
    this->size += 10;
  }
  this->items[this->count] = item;
  this->count++;
}

template <typename T>
T Array<T>::PopLast() {
  if (count == 0) {
    throw std::runtime_error("Index out of bounds.");
  }
  T item = this->items[this->count - 1];
  this->count--;
  return item;
}

template <typename T>
T& Array<T>::Last() {
  return this->items[this->count - 1];
};