#if !defined(DBDS_BSTREE_NODE_H_)
#define DBDS_BSTREE_NODE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef bool (*dbds_btree_value_comp_function)(const void*, const void*);
typedef const void* (*dbds_btree_get_key_function)(const void*);
typedef void* dbds_btree_visitor_function;

typedef struct bstree_node {
  struct bstree_node *left, *right;
  void* data;
} bstree_node;

bstree_node* new_node(const void* value, size_t data_size);

void free_node(bstree_node* n, void (*free_data)(void*));

typedef void (*data_free_function)(void*);

void remove_node(bstree_node** root_ptr, data_free_function free_data);

void remove_tree(bstree_node** root, void (*free_data)(void*));

bstree_node** find_node(bstree_node** root, const void* data,
                        dbds_btree_value_comp_function comp,
                        dbds_btree_get_key_function get_key);

typedef void (*dbds_btree_visitor_invoker_function)(void* visitor, void* data);

void node_preorder(const bstree_node* n, dbds_btree_visitor_function func,
                   dbds_btree_visitor_invoker_function invoker);

void node_postorder(const bstree_node* n, dbds_btree_visitor_function func,
                    dbds_btree_visitor_invoker_function invoker);

void node_inorder(const bstree_node* n, dbds_btree_visitor_function func,
                  dbds_btree_visitor_invoker_function invoker);

#endif  // DBDS_BSTREE_NODE_H_
