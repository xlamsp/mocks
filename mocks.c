#include "mocks.h"

mocks_return_code
mocks_init(uint32_t number_of_threads, uint32_t context_buffer_size)
{
  if (number_of_threads == 0 || context_buffer_size == 0) {
    return mocks_not_initialized;
  }

  return mocks_success;
}

mocks_return_code
mocks_init_thread(
  uint32_t    thread_index,
  pthread_t  *thread_id,
  uint32_t    number_of_expectations)
{
  return mocks_not_initialized;
}

mocks_return_code
mocks_verify(void)
{
  return mocks_not_initialized;
}
