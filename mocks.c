#include "mocks.h"

mocks_return_code
mocks_init(uint32_t number_of_threads, uint32_t context_buffer_size)
{
  return mocks_not_initialized;
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
