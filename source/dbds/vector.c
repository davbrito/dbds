#include "dbds/vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// The max. initial memory is 16 bytes.
#define INITIAL_VECTOR_MEMORY 16

size_t default_initial_vector_capacity(size_t data_size) {
  // Get a default initial capacity according to the `data_size`
  return INITIAL_VECTOR_MEMORY / data_size;
}

size_t calculate_capacity_growth(size_t capacity) {
  return 8 + capacity + capacity / 3;
}

bool vector_needs_shrink(const dbds_vector* v) {
  return v->capacity / 3 > v->size / 2;
}

void reallocate_vector(dbds_vector* v, size_t new_capacity) {
  v->data = realloc(v->data, new_capacity * v->value_size);
  v->capacity = new_capacity;
}

void grow_vector(dbds_vector* v) {
  reallocate_vector(v, calculate_capacity_growth(v->capacity));
}

void shrink_vector(dbds_vector* v) { reallocate_vector(v, v->size); }

DBDS_EXPORT dbds_vector dbds_vector_new(size_t capacity, size_t data_size) {
  capacity = capacity || default_initial_vector_capacity(data_size);
  return (dbds_vector){
      .data = malloc(capacity * data_size),
      .capacity = capacity,
      .size = 0,
      .value_size = data_size,
  };
}

DBDS_EXPORT void dbds_vector_clear(dbds_vector* v) {
  if (v && v->data) {
    free(v->data);
    *v = (dbds_vector){NULL, 0, 0};
  }
}

bool vector_needs_grow(const dbds_vector* v) { return v->capacity == v->size; }

DBDS_EXPORT void* dbds_vector_push_back(dbds_vector* v, const void* value) {
  if (!v) return NULL;
  if (vector_needs_grow(v)) grow_vector(v);
  void* last = (unsigned char*)v->data + v->size * v->value_size;
  v->size++;
  return memcpy(last, value, v->value_size);
}

DBDS_EXPORT void dbds_vector_pop_back(dbds_vector* v) {
  if (v && v->size) {
    v->size--;
  }
}

DBDS_EXPORT void* dbds_vector_get(const dbds_vector* v, size_t i) {
  if (!v || !v->data || i >= v->size) return NULL;
  return &((unsigned char*)v->data)[i * v->value_size];
}
