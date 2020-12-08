#if !defined(DBDS_TESTING_TEST_H_)
#define DBDS_TESTING_TEST_H_

#include <stdbool.h>
#include <stdio.h>

typedef void(test_function)();

typedef struct {
  const char* name;
  test_function* fp;
} test;

const char* test_failed = NULL;

void check(const char* name, bool assertion) {
  if (test_failed) return;
  if (!assertion) test_failed = name;
  printf("- %s: %s\n", name, assertion ? "success" : "failed");
}

void check_i(const char* name, int i, bool assertion) {
  if (test_failed) return;
  if (!assertion) test_failed = name;
  printf("- %s(%i): %s\n", name, i, assertion ? "success" : "failed");
}

bool run_test(const test t) {
  printf("Test %s:\n", t.name);
  t.fp();
  if (test_failed) {
    printf("    ERROR: %s\n", test_failed);
  } else {
    printf("    SUCCESS\n");
  }
  return !test_failed;
}

bool run_tests(const test ts[]) {
  for (const test* i = &ts[0]; i->fp != NULL; ++i) {
    if (!run_test(*i)) return false;
  }
  return true;
}

#endif  // DBDS_TESTING_TEST_H_
