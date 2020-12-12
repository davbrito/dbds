#include "dbds/set.h"

#include <stdlib.h>
#include <string.h>

#include "dbds/set/node.h"

btree_node** get_ptr_to_root(const dbds_set* set) {
  return (btree_node**)&set->data;
}

btree_node* get_root(const dbds_set* set) { return (btree_node*)set->data; }

DBDS_EXPORT dbds_set dbds_set_new(size_t value_size, comp_function comp) {
  return (dbds_set){
      .data = NULL,
      .size = 0,
      .comp = comp,
      .value_size = value_size,
  };
}

DBDS_EXPORT void dbds_set_clear(dbds_set* s) {
  if (!s || !s->data) return;
  remove_tree(get_ptr_to_root(s));
  s->size = 0u;
}

DBDS_EXPORT const void* dbds_set_insert(dbds_set* s, const void* value) {
  if (!s || !value) return NULL;
  btree_node** node = find_node(get_ptr_to_root(s), value, s->comp);
  if (!(*node)) {
    (*node) = new_node(value, s->value_size);
    s->size++;
  }
  return (*node)->data;
}

/// Remove and element of the set.
DBDS_EXPORT void dbds_set_remove(dbds_set* s, const void* value) {
  if (!s || !value) return;
  btree_node** node = find_node(get_ptr_to_root(s), value, s->comp);
  if (*node) {
    remove_node(node);
    s->size--;
  }
}

/// Check if the set contains an element.
DBDS_EXPORT bool dbds_set_contains(const dbds_set* s, const void* value) {
  if (!s || !value) return NULL;
  return *find_node(get_ptr_to_root(s), value, s->comp) != NULL;
}

/// Pre-order traversal on the tree
DBDS_EXPORT void dbds_set_preorder(const dbds_set* s, visitor_function func) {
  node_preorder(get_root(s), func);
}

/// Post-order traversal on the tree
DBDS_EXPORT void dbds_set_postorder(const dbds_set* s, visitor_function func) {
  node_postorder(get_root(s), func);
}

/// In-order traversal on the tree
DBDS_EXPORT void dbds_set_inorder(const dbds_set* s, visitor_function func) {
  node_inorder(get_root(s), func);
}
