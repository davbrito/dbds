#if !defined(DBDS_SET_NODE_H_)
#define DBDS_SET_NODE_H_

#include <stdlib.h>
#include <string.h>

#include "dbds/set.h"

typedef struct btree_node {
  struct btree_node *left, *right;
  void* data;
} btree_node;

btree_node* new_node(const void* value, size_t data_size) {
  btree_node* n = (btree_node*)malloc(sizeof(btree_node));
  void* data = memcpy(malloc(data_size), value, data_size);
  *n = (btree_node){NULL, NULL, data};
  return n;
}

void free_node(btree_node* n) {
  free(n->data);
  free(n);
}

btree_node** get_rigthmost(btree_node** n) {
  // *n is assumed not NULL
  while ((*n)->right) n = &(*n)->right;
  return n;
}

void swap_node_data(btree_node* a, btree_node* b) {
  void* temp = a->data;
  a->data = b->data;
  b->data = temp;
}

void remove_node(btree_node** root_ptr) {
  btree_node* root = *root_ptr;
  if (!root->left && !root->right) {
    free_node(root);
    *root_ptr = NULL;
  } else if (!root->left) {
    *root_ptr = root->right;
    free_node(root);
  } else if (!root->right) {
    *root_ptr = root->left;
    free_node(root);
  } else {
    btree_node** left_rightmost = get_rigthmost(&root->left);
    swap_node_data(root, *left_rightmost);
    remove_node(left_rightmost);
  }
}

void remove_tree(btree_node** root) {
  if (!(*root)) return;
  remove_tree((btree_node**)&(*root)->left);
  remove_tree((btree_node**)&(*root)->right);
  free_node(*root);
  *root = NULL;
}

btree_node** find_node(btree_node** root, const void* data,
                       comp_function comp) {
  if (!(*root)) return root;
  if (comp((*root)->data, data)) {
    return find_node(&(*root)->right, data, comp);
  } else if (comp(data, (*root)->data)) {
    return find_node(&(*root)->left, data, comp);
  }
  return root;
}

void node_preorder(const btree_node* n, dbds_set_visitor_function func) {
  if (n) {
    func(n->data);
    node_preorder(n->left, func);
    node_preorder(n->right, func);
  }
}

void node_postorder(const btree_node* n, dbds_set_visitor_function func) {
  if (n) {
    node_postorder(n->left, func);
    node_postorder(n->right, func);
    func(n->data);
  }
}

void node_inorder(const btree_node* n, dbds_set_visitor_function func) {
  if (n) {
    node_inorder(n->left, func);
    func(n->data);
    node_inorder(n->right, func);
  }
}

#endif  // DBDS_SET_NODE_H_
