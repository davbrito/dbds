#if !defined(DBDS_TEST_VECTOR_H_)
#define DBDS_TEST_VECTOR_H_

#include "dbds/vector.h"
#include "testing/test.h"

test_function test_vector;

void test_vector() {
  dbds_vector vec = dbds_vector_new(0, sizeof(int));
  check("Test empty vector initialization", vec.size == 0);
  const int elements = 20;

  for (int i = 0; i < elements; i++) {
    const int *ptr = dbds_vector_push_back(&vec, &i, sizeof(int));
    check_i("Test push back", i,
            vec.size == i + 1 && vec.capacity != 0 && vec.data != NULL);
    check_i("Test push back return value", i, *ptr == i);
  }

  for (int i = 0; i < elements; i++) {
    const int *ptr = dbds_vector_get(&vec, i, sizeof(int));
    check_i("Test get", i, *ptr == i);
  }

  for (int i = 0; i < elements; i++) {
    dbds_vector_pop_back(&vec);
    check_i("Test pop back", i, vec.size == elements - i - 1);
    int *ptr = dbds_vector_get(&vec, elements - i - 1, sizeof(int));
    check_i("Test pop back then get", i, ptr == NULL);
  }

  dbds_vector_clear(&vec);
  check("Test clear vector",
        vec.size == 0 && vec.capacity == 0 && vec.data == NULL);
}

#endif  // DBDS_TEST_VECTOR_H_
