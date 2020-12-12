#if !defined(DBDS_TEST_SET_H_)
#define DBDS_TEST_SET_H_

#include "dbds/set.h"
#include "testing/test.h"

bool compare_ints(const int *l, const int *r) { return *l < *r; }

#define TEST_NUMS_N 7
#define TEST_NUMS_UNIQUE_N 5

const int nums[TEST_NUMS_N] = {10, 23, 10, 5,
                               6,  23, 543};  // 7 numbers, but 5 unique ones.
// Tree:
//     10
//    /  \.
//  5      23
//   \       \.
//    6       543
// Expected traversals
//  - inorder: 5 6 10 23 543
//  - preorder: 10 5 6 23 43
//  - postorder: 6 5 543 23 10

struct traversal_test_data {
  int i;
  const int *nums;
  bool result;
} tdata;

void test_visitor(const int *num_ptr) {
  tdata.result &= (tdata.nums[tdata.i++] == *num_ptr);
}

bool test_traversal(const dbds_set *set,
                    void (*traversal)(const dbds_set *, visitor_function),
                    const int expected[TEST_NUMS_UNIQUE_N]) {
  tdata.i = 0;
  tdata.nums = expected;
  tdata.result = true;
  traversal(set, (visitor_function)&test_visitor);
  return tdata.result;
}

bool test_inorder(const dbds_set *set) {
  return test_traversal(set, &dbds_set_inorder, (int[]){5, 6, 10, 23, 543});
}

bool test_preorder(const dbds_set *set) {
  return test_traversal(set, &dbds_set_preorder, (int[]){10, 5, 6, 23, 543});
}

bool test_postorder(const dbds_set *set) {
  return test_traversal(set, &dbds_set_postorder, (int[]){6, 5, 543, 23, 10});
}

test_function test_set;
void test_set() {
  dbds_set set = dbds_set_new(sizeof(int), (comp_function)&compare_ints);
  CHECK_EQ_INT("Set initialization: ", set.size, 0);

  int n = 10;

  CHECK("Set don't contains 10", !dbds_set_contains(&set, &n));

  // test `dbds_set_insert`
  for (int i = 0; i < TEST_NUMS_N; i++) {
    int num = nums[i];
    const int *inserted = dbds_set_insert(&set, &num);

    CHECK_I("Set contains", num, dbds_set_contains(&set, &num));
    CHECK_EQ_INT("Set insert returns value: ", *inserted, num);
  }

  CHECK_EQ_UINT("Set size increase: ", set.size, TEST_NUMS_UNIQUE_N);

  // test `dbds_set_contains`
  for (int i = 0; i < TEST_NUMS_N; i++) {
    int num = nums[i];
    CHECK_I("Set contains", num, dbds_set_contains(&set, &num));
  }

  // test `dbds_set_inorder`
  CHECK("Set inorder traversal", test_inorder(&set));
  CHECK("Set preorder traversal", test_preorder(&set));
  CHECK("Set postorder traversal", test_postorder(&set));

  for (int i = 0; i < TEST_NUMS_N; i++) {
    int num = nums[i];
    dbds_set_remove(&set, &num);
    CHECK_I("Set not contains", num, !dbds_set_contains(&set, &num));
  }
  CHECK_EQ_UINT("Set size decrease: ", set.size, 0u);

  dbds_set_clear(&set);
  CHECK_EQ_UINT("Set clear: ", set.size, 0u);
  CHECK_EQ_PTR("Set clear: ", set.data, NULL);
}

#endif  // DBDS_TEST_SET_H_
