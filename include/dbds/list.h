#if !defined(DBDS_LIST_H_)
#define DBDS_LIST_H_

#include <stddef.h>

#include "dbds/export.h"

typedef struct dbds_list_node dbds_list_node;

typedef struct dbds_list {
  dbds_list_node* head;
  dbds_list_node* back;
  size_t size;
  size_t value_size;
} dbds_list;

// Create a new list.
DBDS_EXPORT dbds_list dbds_list_new(size_t value_size);

// Clear the content of the list.
DBDS_EXPORT void dbds_list_clear(dbds_list*);

// Insert an element on the beggining of the list.
DBDS_EXPORT void* dbds_list_push_front(dbds_list*, const void* value);

// Insert an element on the end of the list.
DBDS_EXPORT void* dbds_list_push_back(dbds_list*, const void* value);

// Remove the first element.
DBDS_EXPORT void dbds_list_pop_front(dbds_list*);

// Remove the last element.
DBDS_EXPORT void dbds_list_pop_back(dbds_list*);

typedef void (*dbds_list_visitor_function)(void* value);

// Call the visitor function with a pointer to each element in the list.
DBDS_EXPORT void dbds_list_for_each(dbds_list*, dbds_list_visitor_function);

// Call the visitor function with a pointer to each element in the list
// in reverse order.
DBDS_EXPORT void dbds_list_reverse_for_each(dbds_list*,
                                            dbds_list_visitor_function);

#endif  // DBDS_LIST_H_
