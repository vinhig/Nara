#include <iostream>

#include "../common/Array.h"
#include "TMacros.h"

void checkIntegrity(Array<int>* test) {
  unsigned int size;
  unsigned int count;
  ASSERT_THROW(size >= count);
}

int main(int argc, char const *argv[]) {
  Array<int> test(32);

  ASSERT_EQUAL(test.Size(), 32);
  ASSERT_EQUAL(test.Count(), 0);
  checkIntegrity(&test);

  for (size_t i = 0; i < 78; i++) {
    test.Add(i);
    checkIntegrity(&test);
  }

  for (size_t i = 0; i < 78; i++) {
    ASSERT_EQUAL(test[i], i);
    checkIntegrity(&test);
  }

  ASSERT_EQUAL(test[77], test.Last());
  checkIntegrity(&test);

  for (size_t i = 0; i < 78; i++) {
    unsigned int a = test[test.Count() - 1];
    unsigned int b = test[77 - i];
    ASSERT_EQUAL(a, b);
    checkIntegrity(&test);
    unsigned int last = test.Last();
    unsigned int popLast = test.PopLast();
    ASSERT_EQUAL(last, popLast);
    checkIntegrity(&test);
  }

  ASSERT_EQUAL(test.Count(), 0);
  checkIntegrity(&test);

  return 0;
}
