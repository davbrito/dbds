#if !defined(DBDS_LIST_NODE_H_)
#define DBDS_LIST_NODE_H_

#include <stdlib.h>
#include <string.h>

#include "dbds/list.h"

struct dbds_list_node {
  void* data;
  struct dbds_list_node* prev;
  struct dbds_list_node* next;
};

dbds_list_node* new_list_node(const void* value, size_t value_size,
                         dbds_list_node* prev, dbds_list_node* next) {
  dbds_list_node* node = malloc(sizeof(dbds_list_node));
  *node = (dbds_list_node){
      .data = memcpy(malloc(value_size), value, value_size),
      .prev = prev,
      .next = next,
  };
  if (prev) prev->next = node;
  if (next) next->prev = node;
  return node;
}

void free_list_node(dbds_list_node* node) {
  free(node->data);
  free(node);
}

#endif  // DBDS_LIST_NODE_H_
