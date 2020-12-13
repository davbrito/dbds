#if !defined(DBDS_TEST_VECTOR_H_)
#define DBDS_TEST_VECTOR_H_

#include "dbds/vector.h"
#include "testing/test.h"

TEST_CASE(test_vector, "Test vector") {
  dbds_vector vec = dbds_vector_new(0, sizeof(int));
  CHECK("Test empty vector initialization", vec.size == 0);
  const int elements = 20;

  for (int i = 0; i < elements; i++) {
    const int *ptr = dbds_vector_push_back(&vec, &i);
    CHECK_I("Test push back", i,
            vec.size == i + 1 && vec.capacity != 0 && vec.data != NULL);
    CHECK_I("Test push back return value", i, *ptr == i);
  }

  for (int i = 0; i < elements; i++) {
    const int *ptr = dbds_vector_get(&vec, i);
    CHECK_I("Test get", i, *ptr == i);
  }

  for (int i = 0; i < elements; i++) {
    dbds_vector_pop_back(&vec);
    CHECK_I("Test pop back", i, vec.size == elements - i - 1);
    int *ptr = dbds_vector_get(&vec, elements - i - 1);
    CHECK_I("Test pop back then get", i, ptr == NULL);
  }

  dbds_vector_clear(&vec);
  CHECK("Test clear vector",
        vec.size == 0 && vec.capacity == 0 && vec.data == NULL);
}
TEST_CASE_END()

#endif  // DBDS_TEST_VECTOR_H_
