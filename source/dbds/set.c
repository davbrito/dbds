#include "dbds/set.h"

#include <stdlib.h>
#include <string.h>

#include "dbds/set/node.h"

DBDS_EXPORT dbds_set dbds_set_new(comp_function comp) {
  return (dbds_set){
      .data = NULL,
      .size = 0,
      .comp = comp,
  };
}

DBDS_EXPORT void dbds_set_clear(dbds_set* s) {
  if (!s || !s->data) return;
  remove_tree((btree_node**)&s->data);
  s->size = 0u;
}

DBDS_EXPORT const void* dbds_set_insert(dbds_set* s, const void* value,
                                        size_t data_size) {
  if (!s || !value) return NULL;
  btree_node** node = find_node((btree_node**)&s->data, value, s->comp);
  if (!(*node)) {
    (*node) = new_node(value, data_size);
    s->size++;
  }
  return (*node)->data;
}

/// Remove and element of the set.
// TODO
int i = 0;
DBDS_EXPORT void dbds_set_remove(dbds_set* s, const void* value) {
  if (!s || !value) return;
  btree_node** node = find_node((btree_node**)&s->data, value, s->comp);
  if (*node) {
    remove_node(node);
    printf("**** %d\n", i++);
    s->size--;
  }
}

/// Check if the set contains an element.
DBDS_EXPORT bool dbds_set_contains(const dbds_set* s, const void* value) {
  if (!s || !value) return NULL;
  return *find_node((btree_node**)&s->data, value, s->comp) != NULL;
}

/// Pre-order traversal on the tree
DBDS_EXPORT void dbds_set_preorder(const dbds_set* s, visitor_function func) {
  node_preorder((btree_node*)s->data, func);
}

/// Post-order traversal on the tree
DBDS_EXPORT void dbds_set_postorder(const dbds_set* s, visitor_function func) {
  node_postorder((btree_node*)s->data, func);
}

/// In-order traversal on the tree
DBDS_EXPORT void dbds_set_inorder(const dbds_set* s, visitor_function func) {
  node_inorder((btree_node*)s->data, func);
}
