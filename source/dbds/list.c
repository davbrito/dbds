#include "dbds/list.h"

#include "dbds/list/node.h"

DBDS_EXPORT dbds_list dbds_list_new(size_t value_size) {
  return (dbds_list){
      .head = NULL,
      .back = NULL,
      .size = 0,
      .value_size = value_size,
  };
}

DBDS_EXPORT void dbds_list_clear(dbds_list* list) {
  if (!list) return;

  for (dbds_list_node* node = list->head; node;) {
    dbds_list_node* aux = node->next;
    free_list_node(node);
    node = aux;
  }

  *list = dbds_list_new(list->value_size);
}

DBDS_EXPORT void* dbds_list_push_front(dbds_list* list, const void* value) {
  if (!list) return NULL;

  list->head = new_list_node(value, list->value_size, NULL, list->head);
  if (!list->head->next) list->back = list->head;

  list->size++;
  return list->head->data;
}

DBDS_EXPORT void* dbds_list_push_back(dbds_list* list, const void* value) {
  if (!list) return NULL;

  list->back = new_list_node(value, list->value_size, list->back, NULL);
  if (!list->back->prev) list->head = list->back;

  list->size++;
  return list->back->data;
}

DBDS_EXPORT void dbds_list_pop_front(dbds_list* list) {
  if (!list || !list->head) return;

  list->head = list->head->next;
  free_list_node(list->head->prev);
  list->head->prev = NULL;

  list->size--;
}

DBDS_EXPORT void dbds_list_pop_back(dbds_list* list) {
  if (!list || !list->back) return;

  list->back = list->back->prev;
  free_list_node(list->back->next);
  list->back->next = NULL;

  list->size--;
}

typedef void (*dbds_list_visitor_function)(void* value);

DBDS_EXPORT void dbds_list_for_each(dbds_list* list,
                                    dbds_list_visitor_function fp) {
  for (dbds_list_node* node = list->head; node; node = node->next)
    fp(node->data);
}

DBDS_EXPORT void dbds_list_reverse_for_each(dbds_list* list,
                                            dbds_list_visitor_function fp) {
  for (dbds_list_node* node = list->back; node; node = node->prev)
    fp(node->data);
}
