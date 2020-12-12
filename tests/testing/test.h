#if !defined(DBDS_TESTING_TEST_H_)
#define DBDS_TESTING_TEST_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef void(test_function)();

typedef struct {
  const char* name;
  test_function* fp;
} test;

#define MAX_TEST_DESC 100

char test_failed_desc[MAX_TEST_DESC] = "";

bool __test_failed() { return test_failed_desc[0] != '\0'; }

bool __check(const char* context, bool assertion) {
  if (!assertion) strcpy(test_failed_desc, context);
  return !__test_failed();
}

#define _REPORT_FILE_AND_EXIT_IF(fails)              \
  if (fails) {                                       \
    printf("    on -> %s:%i\n", __FILE__, __LINE__); \
    return;                                          \
  }

#define _CHECK_1(name, assertion)                                         \
  {                                                                       \
    bool __test_passed = __check(name, assertion);                        \
    printf("- %s -> %s\n", name, (__test_passed ? "success" : "failed")); \
    _REPORT_FILE_AND_EXIT_IF(!__test_passed);                             \
  }

#define _CHECK_2(name, assertion, fmt, ...)                     \
  {                                                             \
    bool __test_passed = __check(name, assertion);              \
    printf("- %s: ", name);                                     \
    printf(fmt, __VA_ARGS__);                                   \
    printf(" -> %s\n", (__test_passed ? "success" : "failed")); \
    _REPORT_FILE_AND_EXIT_IF(!__test_passed);                   \
  }

#define CHECK(name, assertion) _CHECK_1(name, assertion)

#define CHECK_I(name, i, assertion) _CHECK_2(name, assertion, "(%i)", i)

#define _CHECK_EQ(name, lho, rho, fmt, eq_cmp)                           \
  {                                                                      \
    bool __assrt = eq_cmp(lho, rho);                                     \
    char __name[MAX_TEST_DESC];                                          \
    sprintf(__name, name #lho " (" fmt ") == " #rho " (" fmt ")", (lho), \
            (rho));                                                      \
    _CHECK_1(__name, __assrt);                                           \
  }

#define _EQ_COMP(lho, rho) ((lho) == (rho))
#define _STREQ_COMP(lho, rho) (strcmp(lho, rho) == 0);

#define CHECK_EQ_INT(name, lho, rho) _CHECK_EQ(name, lho, rho, "%d", _EQ_COMP)
#define CHECK_EQ_UINT(name, lho, rho) _CHECK_EQ(name, lho, rho, "%u", _EQ_COMP)
#define CHECK_EQ_PTR(name, lho, rho) _CHECK_EQ(name, lho, rho, "%p", _EQ_COMP)
#define CHECK_EQ_STR(name, lho, rho) _CHECK_EQ(name, lho, rho, "%s", _STREQ_CMP)

// const char __op[3] = __assrt ? "==" : "!=";

// #define CHECK_EQ_I(name, i, lho, rho)                                       \
//   _CHECK_2(name "(" #lho " == " #rho ")", (lho == rho), "(%i) %d == %d", i, \
//            lho, rho)

bool run_test(const test t) {
  printf("Test %s:\n", t.name);
  t.fp();
  if (__test_failed()) {
    printf("    ERROR: %s\n", test_failed_desc);
  } else {
    printf("    SUCCESS\n");
  }
  return !__test_failed();
}

bool run_tests(const test ts[]) {
  for (const test* i = &ts[0]; i->fp != NULL; ++i) {
    if (!run_test(*i)) return false;
  }
  return true;
}

#endif  // DBDS_TESTING_TEST_H_
