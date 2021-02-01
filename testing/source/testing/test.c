#include "testing/test.h"

#include <stdarg.h>

static char test_failed_description[MAX_TEST_DESC] = "";

bool __test_failed() { return test_failed_description[0] != '\0'; }

bool __check(const char* context, bool assertion) {
  if (!assertion) strcpy(test_failed_description, context);
  return !__test_failed();
}

int __print_check_eq_name(char* dest, size_t destsz, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
#if defined _MSC_VER 
  return sprintf_s(dest, destsz, fmt, args);
#else
  return sprintf(dest, fmt, args);
#endif
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

bool run_tests(test_function_ptr* ts) {
  while (*ts) {
    if (!run_test(*ts++)) return false;
  }
  return true;
}
