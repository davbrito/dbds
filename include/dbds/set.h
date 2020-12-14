#if !defined(DBDS_SET_H_)
#define DBDS_SET_H_

/*
This header defines the interface of a binary search tree data structure.
*/

#include <stdbool.h>
#include <stddef.h>

#include "dbds/export.h"

typedef bool (*comp_function)(const void*, const void*);

typedef struct dbds_set {
  void* data;  // An implementaton defined type.
  size_t size;
  comp_function comp;
  size_t value_size;
} dbds_set;

/// Create a new empty set with the given comparison function.
DBDS_EXPORT dbds_set dbds_set_new(size_t value_size, comp_function);

/// Clears the content of the set and release the allocated memory.
DBDS_EXPORT void dbds_set_clear(dbds_set*);

/// Inserts an element on the set.
DBDS_EXPORT const void* dbds_set_insert(dbds_set*, const void* value);

/// Remove and element of the set.
DBDS_EXPORT void dbds_set_remove(dbds_set*, const void* value);

/// Check if the set contains an element.
DBDS_EXPORT bool dbds_set_contains(const dbds_set*, const void* value);

typedef void (*dbds_set_visitor_function)(const void*);

/// Pre-order traversal on the tree
DBDS_EXPORT void dbds_set_preorder(const dbds_set*, dbds_set_visitor_function);

/// Post-order traversal on the tree
DBDS_EXPORT void dbds_set_postorder(const dbds_set*, dbds_set_visitor_function);

/// In-order traversal on the tree
DBDS_EXPORT void dbds_set_inorder(const dbds_set*, dbds_set_visitor_function);

#endif  // DBDS_SET_H_
