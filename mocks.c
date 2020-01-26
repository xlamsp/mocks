#include "mocks.h"
#include <stdlib.h>
#include <string.h>


typedef struct {
  int expect_count;
  int invoke_count;
  mocks_expectation_t *expectations;
  int max_expectations;
} mocks_thread_t;

static struct {
  int number_of_threads;
  mocks_thread_t thread;

  int context_size;
  int context_used;
  uint8_t *context_data;
} mocks;


mocks_return_code
mocks_init(int number_of_threads, int context_buffer_size)
{
  if (number_of_threads <= 0 || context_buffer_size <= 0) {
    return mocks_not_initialized;
  }

  mocks.number_of_threads = number_of_threads;
  mocks.context_size = context_buffer_size;
  mocks.context_data = malloc(context_buffer_size);
  mocks.context_used = 0;
  return mocks_success;
}

mocks_return_code
mocks_init_thread(
  int         thread_index,
  pthread_t  *thread_id,
  int         number_of_expectations)
{
  if (mocks.number_of_threads <= 0) {
    return mocks_not_initialized;
  }

  if (thread_index >= mocks.number_of_threads) {
    return mocks_thread_index_out_of_range;
  }

  if (!thread_id) {
    return mocks_thread_id_is_null;
  }

  if (number_of_expectations <= 0) {
    return mocks_thread_bad_number_of_expectations;
  }

  mocks.thread.expectations =
    malloc(number_of_expectations * sizeof(mocks.thread.expectations[0]));
  mocks.thread.max_expectations = number_of_expectations;

  return mocks_success;
}

void
mocks_cleanup(void)
{
  mocks.number_of_threads = 0;
  free(mocks.thread.expectations);
  free(mocks.context_data);
}

mocks_return_code
mocks_expect(
  int                         thread_index,
  const mocks_expectation_t  *expected)
{
  mocks_expectation_t *expectation;

  if (!expected) {
    return mocks_invalid_argument;
  }

  if (expected->context_size < 0) {
    return mocks_invalid_ctx_size;
  }

  if (expected->context_size > 0 && !expected->context_data) {
    return mocks_invalid_ctx;
  }

  if (mocks.thread.expect_count >= mocks.thread.max_expectations) {
    return mocks_no_room_for_expectation;
  }

  if (expected->context_size > mocks.context_size) {
    return mocks_no_room_for_ctx_data;
  }

  expectation = &mocks.thread.expectations[mocks.thread.expect_count];

  expectation->id = expected->id;
  expectation->context_size = expected->context_size;
  if (expected->context_size) {
    expectation->context_data = mocks.context_data + mocks.context_used;
    memcpy(
      expectation->context_data,
      expected->context_data,
      expected->context_size);
    mocks.context_used += expected->context_size;
  } else {
    expectation->context_data = NULL;
  }
  mocks.thread.expect_count++;

  return mocks_success;
}

mocks_return_code
mocks_invoke(
  mocks_expectation_t        *invoked)
{
  if (!invoked) {
    return mocks_invalid_argument;
  }

  if (mocks.thread.invoke_count >= mocks.thread.expect_count) {
    return mocks_no_more_expectations;
  }

  *invoked = mocks.thread.expectations[mocks.thread.invoke_count];
  mocks.thread.invoke_count++;

  return mocks_success;
}

mocks_return_code
mocks_verify(void)
{
  if (mocks.number_of_threads <= 0) {
    return mocks_not_initialized;
  }

  return mocks_success;
}
