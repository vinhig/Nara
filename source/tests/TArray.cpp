#include <iostream>

#include "../common/Array.h"
#include "TMacros.h"

void checkIntegrity(Array<int>* test) {
  unsigned int size;
  unsigned int count;
  ASSERT_THROW(size >= count, "Array must have a size greater or equal to count");
}

int main(int argc, char const *argv[]) {
  Array<int> test(32);

  ASSERT_EQUAL(test.Size(), 32, "Array must have a size of two.");
  ASSERT_EQUAL(test.Count(), 0, "Array but have a count of zero.");
  checkIntegrity(&test);

  bool failed = false;
  try
  {
    test[0] = 42;
  }
  catch(const std::exception& e)
  {
    failed = true;
  }
  ASSERT_THROW(failed, "Array must throw after assigning without adding.");
  

  for (size_t i = 0; i < 78; i++) {
    test.Add(i);
    checkIntegrity(&test);
  }

  for (size_t i = 0; i < 78; i++) {
    ASSERT_EQUAL(test[i], i, "Array must return the correct number.");
    checkIntegrity(&test);
  }

  ASSERT_EQUAL(test[77], test.Last(), "Array must return the last number.");
  checkIntegrity(&test);

  for (size_t i = 0; i < 78; i++) {
    unsigned int a = test[test.Count() - 1];
    unsigned int b = test[77 - i];
    ASSERT_EQUAL(a, b, "Array must return a number corresponding to its count.");
    checkIntegrity(&test);
    unsigned int last = test.Last();
    unsigned int popLast = test.PopLast();
    ASSERT_EQUAL(last, popLast, "Array must return the last number when being PopLast.");
    checkIntegrity(&test);
  }

  ASSERT_EQUAL(test.Count(), 0, "Array must have a count of zero after removing every elements.");
  checkIntegrity(&test);

  return 0;
}
