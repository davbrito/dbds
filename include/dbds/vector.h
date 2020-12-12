#if !defined(DBDS_VECTOR_H_)
#define DBDS_VECTOR_H_

/*
This header defines a heap allocated vector data structure and its interface.
*/

#include <stddef.h>

#include "dbds/export.h"

typedef struct {
  void* data;
  size_t size;
  size_t capacity;
  size_t value_size;
} dbds_vector;

/// Create a new empty vector with an initial preallocated capacity
DBDS_EXPORT dbds_vector dbds_vector_new(size_t capacity, size_t data_size);

/// Clears the content of the vector and release the allocated memory
DBDS_EXPORT void dbds_vector_clear(dbds_vector*);

/// Inserts an element on the end of the vector.
DBDS_EXPORT void* dbds_vector_push_back(dbds_vector*, const void* value);

/// Remove the element on the end of the vector.
DBDS_EXPORT void dbds_vector_pop_back(dbds_vector*);

/// Retrieves the ith element on the vector.
DBDS_EXPORT void* dbds_vector_get(const dbds_vector*, size_t i);

#endif  // DBDS_VECTOR_H_
