//
// Created by vincent on 24.08.20.
//

#ifndef NARA_SOURCE_COMMON_ARRAY_H_
#define NARA_SOURCE_COMMON_ARRAY_H_

#include <iostream>

/**
 * Extensible array.
 * Automatically reallocate memory when adding item.
 * Can add item.
 * Cannot remove item.
 */
template <typename T>
class Array {
 private:
  T* items;
  unsigned int count;
  unsigned int size;

 public:
  /**
   * Create an extensible array with a given memory size.
   * this->count = 0
   */
  Array(unsigned int size);
  /**
   * Delete the extensible array and free memory
   */
  ~Array();
  /**
   * Add a new item.
   * Reallocate memory if this->size < this.count + 1.
   */
  void Add(T item);
  /**
   * Return the last item.
   * Reallocate memory if this->size < this.count + 1.
   */
  T& Last();
  /**
   * Remove the last item.
   * Reallocate memory if this->size < this.count + 1.
   */
  T PopLast();
  unsigned int Size() const { return this->size; };
  unsigned int Count() const { return this->count; };
  /**
   * Return adress to i-th item.
   */
  T& operator[](int i) {
    if (i >= count) {
      throw std::runtime_error("Index out of bounds.");
    }
    return items[i];
  }
};

template class Array<unsigned int>;
template class Array<int>;

#endif  // NARA_SOURCE_COMMON_ARRAY_H_