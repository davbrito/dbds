#include "testing/test.h"

bool __test_failed() { return test_failed_description[0] != '\0'; }

bool __check(const char* context, bool assertion) {
  if (!assertion) strcpy(test_failed_description, context);
  return !__test_failed();
}

bool run_test(test_function_ptr fp) {
  const test* t = fp();
  printf("Test %s: (%s:%u)\n", t->name, t->file, t->line);

  bool failed = __test_failed();
  if (failed) {
    printf("    ERROR: %s\n", test_failed_description);
  } else {
    printf("    SUCCESS\n");
  }
  return !failed;
}

void run_tests(test_function_ptr* ts) {
  while (*ts) {
    if (!run_test(*ts++)) break;
  }
}
