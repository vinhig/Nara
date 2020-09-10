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
  T* items = nullptr;
  unsigned int count = 0;
  unsigned int size = 0;

 public:
  Array() = default;
  /**
   * Create an extensible array with a given memory size.
   * this->count = 0
   */
  Array(unsigned int size) {
    this->items = (T*)malloc(sizeof(T) * size);
    this->size = size;
    this->count = 0;
  };
  ;
  /**
   * Delete the extensible array and free memory.
   */
  ~Array() = default;
  /**
   * Add a new item.
   * Reallocate memory if this->size < this.count + 1.
   */
  void Add(T item) {
    if (size = 0) {
      this->items = (T*)realloc(this->items, sizeof(T) * 10);
      this->size = 10;
    } else if (size == count) {
      this->items = (T*)realloc(this->items, sizeof(T) * (size + 10));
      this->size += 10;
    }
    this->items[this->count] = item;
    this->count++;
  }
  /**
   * Delete the extensible array and free memory.
   */
  void Dispose() {
    free(this->items);
    this->items = nullptr;
    this->count = 0;
    this->size = 0;
  }
  /**
   * Return adress to i-th item.
   */
  T& Get(int i) {
    if (i >= count) {
      throw std::runtime_error("Index out of bounds.");
    }
    return items[i];
  }
  /**
   * Return the last item.
   * Reallocate memory if this->size < this.count + 1.
   */
  T& Last() { return this->items[this->count - 1]; }
  /**
   * Remove the last item.
   * Reallocate memory if this->size < this.count + 1.
   */
  T PopLast() {
    if (count == 0) {
      throw std::runtime_error("Index out of bounds.");
    }
    T item = this->items[this->count - 1];
    this->count--;
    return item;
  }
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

#endif  // NARA_SOURCE_COMMON_ARRAY_H_