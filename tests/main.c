#include "test_vector.h"
#include "testing/test.h"

const test tests[] = {
    {"Test vector", &test_vector},
    {NULL, NULL},
};

int main() { run_tests(tests); }
