#include "dbds/bstree/node.h"

bstree_node* new_node(const void* value, size_t data_size) {
  bstree_node* n = (bstree_node*)malloc(sizeof(bstree_node));
  *n = (bstree_node){
      .left = NULL,
      .right = NULL,
      .data = memcpy(malloc(data_size), value, data_size),
  };
  return n;
}

void free_node(bstree_node* n, void (*free_data)(void*)) {
  if (!free_data) free_data = &free;
  free_data(n->data);
  free(n);
}

bstree_node** get_rigthmost(bstree_node** n) {
  // *n is assumed not NULL
  while ((*n)->right) n = &(*n)->right;
  return n;
}

void swap_node_data(bstree_node* a, bstree_node* b) {
  void* temp = a->data;
  a->data = b->data;
  b->data = temp;
}

typedef void (*data_free_function)(void*);

void remove_node(bstree_node** root_ptr, data_free_function free_data) {
  bstree_node* root = *root_ptr;
  if (!root->left && !root->right) {
    free_node(root, free_data);
    *root_ptr = NULL;
  } else if (!root->left) {
    *root_ptr = root->right;
    free_node(root, free_data);
  } else if (!root->right) {
    *root_ptr = root->left;
    free_node(root, free_data);
  } else {
    bstree_node** left_rightmost = get_rigthmost(&root->left);
    swap_node_data(root, *left_rightmost);
    remove_node(left_rightmost, free_data);
  }
}

void remove_tree(bstree_node** root, void (*free_data)(void*)) {
  if (!(*root)) return;
  remove_tree((bstree_node**)&(*root)->left, free_data);
  remove_tree((bstree_node**)&(*root)->right, free_data);
  free_node(*root, free_data);
  *root = NULL;
}

const void* _identity(const void* x) { return x; };

bstree_node** find_node(bstree_node** root, const void* data,
                        dbds_btree_value_comp_function comp,
                        dbds_btree_get_key_function get_key) {
  if (!(*root)) return root;
  if (!get_key) get_key = &_identity;

  const void* root_data = get_key((*root)->data);

  if (comp(root_data, data)) {
    return find_node(&(*root)->right, data, comp, get_key);
  } else if (comp(data, root_data)) {
    return find_node(&(*root)->left, data, comp, get_key);
  }

  return root;
}

void _default_invoker(void (*func)(const void*), const void* data) {
  func(data);
};

void node_preorder(const bstree_node* n, dbds_btree_visitor_function func,
                   dbds_btree_visitor_invoker_function invoker) {
  if (n) {
    invoker ? invoker(func, n->data) : _default_invoker(func, n->data);
    node_preorder(n->left, func, invoker);
    node_preorder(n->right, func, invoker);
  }
}

void node_postorder(const bstree_node* n, dbds_btree_visitor_function func,
                    dbds_btree_visitor_invoker_function invoker) {
  if (n) {
    node_postorder(n->left, func, invoker);
    node_postorder(n->right, func, invoker);
    invoker ? invoker(func, n->data) : _default_invoker(func, n->data);
  }
}

void node_inorder(const bstree_node* n, dbds_btree_visitor_function func,
                  dbds_btree_visitor_invoker_function invoker) {
  if (n) {
    node_inorder(n->left, func, invoker);
    invoker ? invoker(func, n->data) : _default_invoker(func, n->data);
    node_inorder(n->right, func, invoker);
  }
}
