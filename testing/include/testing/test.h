#if !defined(DBDS_TESTING_TEST_H_)
#define DBDS_TESTING_TEST_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct test {
  const char* name;
  const char* file;
  unsigned line;
} test;

typedef const test*(test_function)();
typedef test_function* test_function_ptr;

#define MAX_TEST_DESC 100

static char test_failed_description[MAX_TEST_DESC] = "";

bool __test_failed();
bool __check(const char* context, bool assertion);

#define TEST_DATA_VAR_NAME __test_data
#define RETURN_TEST_CASE return &TEST_DATA_VAR_NAME

#define _REPORT_FILE_AND_EXIT_IF(fails)              \
  if (fails) {                                       \
    printf("    on -> %s:%i\n", __FILE__, __LINE__); \
    RETURN_TEST_CASE;                                \
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
#define CHECK_EQ_CHAR(name, lho, rho) _CHECK_EQ(name, lho, rho, "%c", _EQ_COMP)
#define CHECK_EQ_PTR(name, lho, rho) _CHECK_EQ(name, lho, rho, "%p", _EQ_COMP)
#define CHECK_EQ_STR(name, lho, rho) _CHECK_EQ(name, lho, rho, "%s", _STREQ_CMP)

#define TEST_CASE_FUNC(f, name_, file_, line_)     \
  test_function f;                                 \
  const test* f() {                                \
    static const test TEST_DATA_VAR_NAME = (test){ \
        .name = name_,                             \
        .file = file_,                             \
        .line = line_,                             \
    };

#define TEST_CASE(fname, name) TEST_CASE_FUNC(fname, name, __FILE__, __LINE__)

#define TEST_CASE_END() \
  RETURN_TEST_CASE;     \
  }

#define DECLARE_TEST_FUNCTIONS(...) test_function __VA_ARGS__;

#define REGISTER_TESTS(...)                             \
  DECLARE_TEST_FUNCTIONS(__VA_ARGS__)                   \
  test_function_ptr __test_set[] = {__VA_ARGS__, NULL}; \
  int main() { return run_tests(__test_set) ? EXIT_SUCCESS : EXIT_FAILURE; }

// Execute a test function
bool run_test(test_function_ptr fp);

// Execute several test functions
bool run_tests(test_function_ptr* ts);

#endif  // DBDS_TESTING_TEST_H_
