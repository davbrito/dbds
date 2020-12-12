#include "test_set.h"
#include "test_vector.h"
#include "testing/test.h"

const test tests[] = {
    {"Test vector", &test_vector},
    {"Test set", &test_set},
    {NULL, NULL},
};

int main() { run_tests(tests); }
