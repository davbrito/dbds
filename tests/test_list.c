#include "dbds/list.h"
#include "testing/test.h"

TEST_CASE(test_list, "Test list") {
  dbds_list list = dbds_list_new(sizeof(int));
  CHECK_EQ_UINT("Empty list init.", list.size, 0);

  for (int i = 0; i < 5; i++) {
    int *ptr = dbds_list_push_back(&list, &i);
    CHECK_EQ_INT("Push back return.", *ptr, i);
  }
  CHECK_EQ_UINT("Size increases.", list.size, 5);

  for (int i = 5; i < 10; i++) {
    int *ptr = dbds_list_push_front(&list, &i);
    CHECK_EQ_INT("Push front return.", *ptr, i);
  }
  CHECK_EQ_UINT("Size increases.", list.size, 10);

  for (int i = 0; i < 3; i++) {
    dbds_list_pop_front(&list);
  }
  CHECK_EQ_UINT("Size decreases.", list.size, 7);

  for (int i = 0; i < 3; i++) {
    dbds_list_pop_back(&list);
  }
  CHECK_EQ_UINT("Size decreases.", list.size, 4);

  dbds_list_clear(&list);
  CHECK_EQ_UINT("Size decreases.", list.size, 0);
}
TEST_CASE_END()
