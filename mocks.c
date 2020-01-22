#include "mocks.h"

static uint32_t mocks_number_of_threads;
static int expect_count;


mocks_return_code
mocks_init(uint32_t number_of_threads, uint32_t context_buffer_size)
{
  if (number_of_threads == 0 || context_buffer_size == 0) {
    return mocks_not_initialized;
  }

  mocks_number_of_threads = number_of_threads;
  return mocks_success;
}

mocks_return_code
mocks_init_thread(
  uint32_t    thread_index,
  pthread_t  *thread_id,
  uint32_t    number_of_expectations)
{
  if (mocks_number_of_threads == 0) {
    return mocks_not_initialized;
  }

  if (thread_index >= mocks_number_of_threads) {
    return mocks_thread_index_out_of_range;
  }

  if (!thread_id) {
    return mocks_thread_id_is_null;
  }

  if (number_of_expectations == 0) {
    return mocks_thread_bad_number_of_expectations;
  }

  return mocks_success;
}

void
mocks_cleanup(void)
{
  mocks_number_of_threads = 0;
}

mocks_return_code
mocks_expect(
  uint32_t    thread_index,
  uint32_t    expectation_id,
  uint32_t    context_size,
  void       *context_data)
{
  expect_count = 1;
  return mocks_success;
}

mocks_return_code
mocks_invoke(
  uint32_t   *expectation_id,
  uint32_t   *context_size,
  void      **context_data)
{
  if (!expect_count) {
    return mocks_no_more_expectations;
  }

  *expectation_id = 0;
  *context_size = 0;
  *context_data = NULL;

  return mocks_success;
}

mocks_return_code
mocks_verify(void)
{
  if (mocks_number_of_threads == 0) {
    return mocks_not_initialized;
  }

  return mocks_success;
}
