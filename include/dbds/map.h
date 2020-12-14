#if !defined(DBDS_MAP_H_)
#define DBDS_MAP_H_

/*
This header defines the interface of a binary search tree data structure.
*/

#include <stdbool.h>
#include <stddef.h>

#include "dbds/export.h"

typedef bool (*dbds_map_key_comp_function)(const void*, const void*);

typedef struct dbds_map {
  void* data;  // An implementaton defined type.
  size_t size;
  dbds_map_key_comp_function comp;
  size_t key_size;
  size_t value_size;
} dbds_map;

/// Create a new empty map with the given key comparison function.
DBDS_EXPORT dbds_map dbds_map_new(size_t key_size, size_t value_size,
                                  dbds_map_key_comp_function);

/// Clears the content of the map and release the allocated memory.
DBDS_EXPORT void dbds_map_clear(dbds_map*);

/// Inserts an element on the map in the given key.
DBDS_EXPORT void* dbds_map_get(dbds_map*, const void* key);

/// Inserts an element on the map in the given key.
DBDS_EXPORT void* dbds_map_insert(dbds_map*, const void* key,
                                  const void* value);

/// Remove the element with the given key of the map.
DBDS_EXPORT void dbds_map_remove(dbds_map*, const void* key);

/// Check if the map contains an element in the given key.
DBDS_EXPORT bool dbds_map_contains(const dbds_map*, const void* key);

typedef void (*dbds_map_visitor_function)(const void* key, void* value);

/// Pre-order traversal on the tree
DBDS_EXPORT void dbds_map_preorder(const dbds_map*, dbds_map_visitor_function);

/// Post-order traversal on the tree
DBDS_EXPORT void dbds_map_postorder(const dbds_map*, dbds_map_visitor_function);

/// In-order traversal on the tree
DBDS_EXPORT void dbds_map_inorder(const dbds_map*, dbds_map_visitor_function);

// Create a comparison function.
#define DBDS_MAP_CREATE_DEFAULT_COMP(fname, type) \
  bool fname(const type* lho, const type* rho) { return *lho < *rho; }

#endif  // DBDS_MAP_H_
