#include "dbds/map.h"

#include "dbds/bstree/node.h"
#include "dbds/map/map_item.h"

static inline bstree_node** get_ptr_to_root(const dbds_map* map) {
  return (bstree_node**)&map->data;
}

static inline bstree_node* get_root(const dbds_map* map) {
  return (bstree_node*)map->data;
}

DBDS_EXPORT dbds_map dbds_map_new(size_t key_size, size_t value_size,
                                  dbds_map_key_comp_function comp) {
  return (dbds_map){
      .data = NULL,
      .size = 0,
      .comp = comp,
      .key_size = key_size,
      .value_size = value_size,
  };
}

DBDS_EXPORT void dbds_map_clear(dbds_map* map) {
  if (!map || !map->data) return;
  remove_tree(get_ptr_to_root(map), (data_free_function)&free_map_item);
  map->size = 0u;
}

DBDS_EXPORT void* dbds_map_get(dbds_map* map, const void* key) {
  if (!map || !key) return NULL;
  bstree_node* node = *find_node(get_ptr_to_root(map), key, map->comp,
                                 (dbds_btree_get_key_function)&map_item_key);
  if (node) {
    map_item* item = node->data;
    return item->value;
  }
  return NULL;
}

DBDS_EXPORT void* dbds_map_insert(dbds_map* map, const void* key,
                                  const void* value) {
  if (!map || !key || !value) return NULL;

  bstree_node** node = find_node(get_ptr_to_root(map), key, map->comp,
                                 (dbds_btree_get_key_function)&map_item_key);
  if (!(*node)) {
    map_item it = new_map_item(key, map->key_size, value, map->value_size);
    (*node) = new_node(&it, sizeof(map_item));
    map->size++;
  }
  return ((map_item*)(*node)->data)->value;
}

DBDS_EXPORT void dbds_map_remove(dbds_map* map, const void* key) {
  if (!map || !key) return;
  bstree_node** node = find_node(get_ptr_to_root(map), key, map->comp,
                                 (dbds_btree_get_key_function)&map_item_key);
  if (*node) {
    remove_node(node, (data_free_function)&free_map_item);
    map->size--;
  }
}

DBDS_EXPORT bool dbds_map_contains(const dbds_map* map, const void* key) {
  if (!map || !key) return false;
  return *find_node(get_ptr_to_root(map), key, map->comp,
                    (dbds_btree_get_key_function)&map_item_key) != NULL;
}

void map_item_visitor_invoker(dbds_map_visitor_function func, map_item* i) {
  func(i->key, i->value);
}

/// Pre-order traversal on the tree
DBDS_EXPORT void dbds_map_preorder(const dbds_map* map,
                                   dbds_map_visitor_function func) {
  node_preorder(get_root(map), func,
                (dbds_btree_visitor_invoker_function)&map_item_visitor_invoker);
}

/// Post-order traversal on the tree
DBDS_EXPORT void dbds_map_postorder(const dbds_map* map,
                                    dbds_map_visitor_function func) {
  node_postorder(
      get_root(map), func,
      (dbds_btree_visitor_invoker_function)&map_item_visitor_invoker);
}

/// In-order traversal on the tree
DBDS_EXPORT void dbds_map_inorder(const dbds_map* map,
                                  dbds_map_visitor_function func) {
  node_inorder(get_root(map), func,
               (dbds_btree_visitor_invoker_function)&map_item_visitor_invoker);
}