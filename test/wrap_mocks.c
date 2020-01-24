#include "unity_fixture_malloc_overrides.h"

#include "mocks.c"

#include <string.h>

/*
 * Reset mocks static variables to the state as at the time of the
 * program startup.
 */
void test_mocks_reset_static_variables(void)
{
  mocks_number_of_threads = 0;
  memset(&mocks_thread, 0, sizeof(mocks_thread));
}
