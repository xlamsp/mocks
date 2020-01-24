#include "unity_fixture_malloc_overrides.h"

#include "mocks.c"

/*
 * Reset mocks static variables to the state as at the time of the
 * program startup.
 */
void test_mocks_reset_static_variables(void)
{
  mocks_number_of_threads = 0;
  expect_count = 0;
  invoke_count = 0;
  expectations = NULL;
  max_expectations = 0;
}
