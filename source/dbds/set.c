#include "dbds/set.h"

#include <stdlib.h>
#include <string.h>

#include "dbds/bstree/node.h"

static inline bstree_node** get_ptr_to_root(const dbds_set* set) {
  return (bstree_node**)&set->data;
}

static inline bstree_node* get_root(const dbds_set* set) { return (bstree_node*)set->data; }

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
  remove_tree(get_ptr_to_root(s), NULL);
  s->size = 0u;
}

DBDS_EXPORT const void* dbds_set_insert(dbds_set* s, const void* value) {
  if (!s || !value) return NULL;
  bstree_node** node = find_node(get_ptr_to_root(s), value, s->comp, NULL);
  if (!(*node)) {
    (*node) = new_node(value, s->value_size);
    s->size++;
  }
  return (*node)->data;
}

DBDS_EXPORT void dbds_set_remove(dbds_set* s, const void* value) {
  if (!s || !value) return;
  bstree_node** node = find_node(get_ptr_to_root(s), value, s->comp, NULL);
  if (*node) {
    remove_node(node, NULL);
    s->size--;
  }
}

DBDS_EXPORT bool dbds_set_contains(const dbds_set* s, const void* value) {
  if (!s || !value) return NULL;
  return *find_node(get_ptr_to_root(s), value, s->comp, NULL) != NULL;
}

DBDS_EXPORT void dbds_set_preorder(const dbds_set* s,
                                   dbds_set_visitor_function func) {
  node_preorder(get_root(s), func, NULL);
}

DBDS_EXPORT void dbds_set_postorder(const dbds_set* s,
                                    dbds_set_visitor_function func) {
  node_postorder(get_root(s), func, NULL);
}

DBDS_EXPORT void dbds_set_inorder(const dbds_set* s,
                                  dbds_set_visitor_function func) {
  node_inorder(get_root(s), func, NULL);
}
