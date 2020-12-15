#if !defined(DBDS_MAP_MAP_ITEM_H_)
#define DBDS_MAP_MAP_ITEM_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct map_item {
  const void* key;
  void* value;
} map_item;

map_item new_map_item(const void* key, size_t key_size, const void* value,
                      size_t value_size) {
  return (map_item){
      .key = memcpy(malloc(key_size), key, key_size),
      .value = memcpy(malloc(value_size), value, value_size),
  };
}

void free_map_item(map_item* i) {
  free((void*)i->key);
  free(i->value);
  free(i);
}

const void* map_item_key(const map_item* i) { return i->key; }

#endif  // DBDS_MAP_MAP_ITEM_H_
