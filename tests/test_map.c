#include <testing/test.h>

#include "dbds/map.h"

DBDS_MAP_CREATE_DEFAULT_COMP(comp_ints, int)

TEST_CASE(test_map, "Test map") {
  dbds_map map = dbds_map_new(sizeof(int), sizeof(char),
                              (dbds_map_key_comp_function)&comp_ints);
  CHECK_EQ_UINT("Map initializes with size 0", map.size, 0);

  char value = 'a';
  for (int key = 0; key < 10; key++) {
    char* ptr = dbds_map_insert(&map, &key, &value);
    CHECK_EQ_CHAR("Map insert return value", *ptr, value);
    CHECK_I("Map contains", key, dbds_map_contains(&map, &key));
    ptr = dbds_map_get(&map, &key);
    CHECK_EQ_CHAR("Map get", *ptr, value);
    value++;
  }
  CHECK_EQ_UINT("Map increase size", map.size, 10);

  value = 'a';
  for (int key = 0; key < 5; key++) {
    CHECK_I("Map contains", key, dbds_map_contains(&map, &key));
    char* ptr = dbds_map_get(&map, &key);
    CHECK_EQ_CHAR("Map get", *ptr, value);
    dbds_map_remove(&map, &key);
    CHECK_I("Map no contains", key, !dbds_map_contains(&map, &key));
    CHECK_EQ_PTR("Map get", dbds_map_get(&map, &key), NULL);
    value++;
  }
  CHECK_EQ_UINT("Map decrease size", map.size, 5);

  dbds_map_clear(&map);
  CHECK_EQ_UINT("Map cleared (size)", map.size, 0);
  CHECK_EQ_UINT("Map cleared (data)", map.data, NULL);
}
TEST_CASE_END()