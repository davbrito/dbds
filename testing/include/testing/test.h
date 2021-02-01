#if !defined(DBDS_TESTING_TEST_H_)
#define DBDS_TESTING_TEST_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// COMPILER VERSION
// ideas for the version stuff are taken from here:
// https://github.com/cxxstuff/cxx_detect

#define TESTING_COMPILER(MAJOR, MINOR, PATCH) \
  ((MAJOR)*10000000 + (MINOR)*100000 + (PATCH))

// GCC/Clang and GCC/MSVC are mutually exclusive, but Clang/MSVC are not because
// of clang-cl...
#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
#if _MSC_VER == _MSC_FULL_VER / 10000
#define TESTING_MSVC \
  TESTING_COMPILER(_MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 10000)
#else  // MSVC
#define TESTING_MSVC                                               \
  TESTING_COMPILER(_MSC_VER / 100, (_MSC_FULL_VER / 100000) % 100, \
                   _MSC_FULL_VER % 100000)
#endif  // MSVC
#endif  // MSVC
#if defined(__clang__) && defined(__clang_minor__)
#define TESTING_CLANG \
  TESTING_COMPILER(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    defined(__GNUC_PATCHLEVEL__) && !defined(__INTEL_COMPILER)
#define TESTING_GCC \
  TESTING_COMPILER(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#endif  // GCC

#ifndef TESTING_MSVC
#define TESTING_MSVC 0
#endif  // TESTING_MSVC
#ifndef TESTING_CLANG
#define TESTING_CLANG 0
#endif  // TESTING_CLANG
#ifndef TESTING_GCC
#define TESTING_GCC 0
#endif  // TESTING_GCC

// COMPILER WARNINGS HELPERS

#if TESTING_CLANG
#define TESTING_PRAGMA_TO_STR(x) _Pragma(#x)
#define TESTING_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
#define TESTING_CLANG_SUPPRESS_WARNING(w) \
  TESTING_PRAGMA_TO_STR(clang diagnostic ignored w)
#define TESTING_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#define TESTING_CLANG_SUPPRESS_WARNING_WITH_PUSH(w) \
  TESTING_CLANG_SUPPRESS_WARNING_PUSH TESTING_CLANG_SUPPRESS_WARNING(w)
#else  // TESTING_CLANG
#define TESTING_CLANG_SUPPRESS_WARNING_PUSH
#define TESTING_CLANG_SUPPRESS_WARNING(w)
#define TESTING_CLANG_SUPPRESS_WARNING_POP
#define TESTING_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif  // TESTING_CLANG

#if TESTING_GCC
#define TESTING_PRAGMA_TO_STR(x) _Pragma(#x)
#define TESTING_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
#define TESTING_GCC_SUPPRESS_WARNING(w) \
  TESTING_PRAGMA_TO_STR(GCC diagnostic ignored w)
#define TESTING_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#define TESTING_GCC_SUPPRESS_WARNING_WITH_PUSH(w) \
  TESTING_GCC_SUPPRESS_WARNING_PUSH TESTING_GCC_SUPPRESS_WARNING(w)
#else  // TESTING_GCC
#define TESTING_GCC_SUPPRESS_WARNING_PUSH
#define TESTING_GCC_SUPPRESS_WARNING(w)
#define TESTING_GCC_SUPPRESS_WARNING_POP
#define TESTING_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif  // TESTING_GCC

#if TESTING_MSVC
#define TESTING_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define TESTING_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#define TESTING_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#define TESTING_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) \
  TESTING_MSVC_SUPPRESS_WARNING_PUSH TESTING_MSVC_SUPPRESS_WARNING(w)
#else  // TESTING_MSVC
#define TESTING_MSVC_SUPPRESS_WARNING_PUSH
#define TESTING_MSVC_SUPPRESS_WARNING(w)
#define TESTING_MSVC_SUPPRESS_WARNING_POP
#define TESTING_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif  // TESTING_MSVC

// WARNING SUPPRESS
TESTING_GCC_SUPPRESS_WARNING_PUSH
TESTING_GCC_SUPPRESS_WARNING("-Wformat")
TESTING_GCC_SUPPRESS_WARNING("-Wformat-extra-args")

TESTING_CLANG_SUPPRESS_WARNING_PUSH
TESTING_GCC_SUPPRESS_WARNING("-Wformat-extra-args")

typedef struct test {
  const char* name;
  const char* file;
  unsigned line;
} test;

typedef const test*(test_function)();
typedef test_function* test_function_ptr;

#define MAX_TEST_DESC 100

bool __test_failed();
bool __check(const char* context, bool assertion);

#define TESTING_FORMAT_TO_STR(fmt) #fmt

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

int __print_check_eq_name(char* dest, size_t destsz, const char* fmt, ...);

#define _CHECK_EQ(name, lho, rho, fmt, eq_cmp)                                 \
  {                                                                            \
    bool __assrt = eq_cmp(lho, rho);                                           \
    char __name[MAX_TEST_DESC];                                                \
    __print_check_eq_name(__name, MAX_TEST_DESC, EQ_FORMAT_STR(fmt), name, #lho, (lho), #rho, \
                          (rho));                                              \
    _CHECK_1(__name, __assrt);                                                 \
  }

#define _EQ_COMP(lho, rho) ((lho) == (rho))
#define _STREQ_COMP(lho, rho) (strcmp(lho, rho) == 0);

// clang-format off
#define EQ_FORMAT_STR(fmt) TESTING_FORMAT_TO_STR(%s: %s (fmt) == %s (fmt))

#define CHECK_EQ_INT(name, lho, rho) _CHECK_EQ(name, lho, rho, %jd, _EQ_COMP)
#define CHECK_EQ_UINT(name, lho, rho) _CHECK_EQ(name, lho, rho, %ju, _EQ_COMP)
#define CHECK_EQ_CHAR(name, lho, rho) _CHECK_EQ(name, lho, rho, '%c', _EQ_COMP)
#define CHECK_EQ_PTR(name, lho, rho) _CHECK_EQ(name, lho, rho, %p, _EQ_COMP)
#define CHECK_EQ_STR(name, lho, rho) _CHECK_EQ(name, lho, rho, %s, _STREQ_CMP)
// clang-format on

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

TESTING_CLANG_SUPPRESS_WARNING_POP
TESTING_MSVC_SUPPRESS_WARNING_POP
TESTING_GCC_SUPPRESS_WARNING_POP

#endif  // DBDS_TESTING_TEST_H_
