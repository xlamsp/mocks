#include "mocks.h"
#include <stdlib.h>

static int mocks_number_of_threads;

typedef struct {
  int expect_count;
  int invoke_count;
  mocks_expectation_t *expectations;
  int max_expectations;
} mocks_thread_t;


static mocks_thread_t mocks_thread;

mocks_return_code
mocks_init(int number_of_threads, int context_buffer_size)
{
  if (number_of_threads <= 0 || context_buffer_size <= 0) {
    return mocks_not_initialized;
  }

  mocks_number_of_threads = number_of_threads;
  return mocks_success;
}

mocks_return_code
mocks_init_thread(
  int         thread_index,
  pthread_t  *thread_id,
  int         number_of_expectations)
{
  if (mocks_number_of_threads <= 0) {
    return mocks_not_initialized;
  }

  if (thread_index >= mocks_number_of_threads) {
    return mocks_thread_index_out_of_range;
  }

  if (!thread_id) {
    return mocks_thread_id_is_null;
  }

  if (number_of_expectations <= 0) {
    return mocks_thread_bad_number_of_expectations;
  }

  mocks_thread.expectations =
    malloc(number_of_expectations * sizeof(mocks_thread.expectations[0]));
  mocks_thread.max_expectations = number_of_expectations;

  return mocks_success;
}

void
mocks_cleanup(void)
{
  mocks_number_of_threads = 0;
  free(mocks_thread.expectations);
}

mocks_return_code
mocks_expect(
  int                         thread_index,
  const mocks_expectation_t  *expectation)
{
  mocks_expectation_t *exp;

  if (expectation->context_size < 0) {
    return mocks_invalid_ctx_size;
  }

  if (expectation->context_size > 0 && !expectation->context_data) {
    return mocks_invalid_ctx;
  }

  if (mocks_thread.expect_count >= mocks_thread.max_expectations) {
    return mocks_no_room_for_expectation;
  }

  exp = &mocks_thread.expectations[mocks_thread.expect_count];

  exp->id = expectation->id;
  mocks_thread.expect_count++;

  return mocks_success;
}

mocks_return_code
mocks_invoke(
  mocks_expectation_t        *expectation)
{
  mocks_expectation_t *exp;

  if (mocks_thread.invoke_count >= mocks_thread.expect_count) {
    return mocks_no_more_expectations;
  }

  exp = &mocks_thread.expectations[mocks_thread.invoke_count];

  expectation->id = exp->id;
  expectation->context_size = 0;
  expectation->context_data = NULL;
  mocks_thread.invoke_count++;

  return mocks_success;
}

mocks_return_code
mocks_verify(void)
{
  if (mocks_number_of_threads <= 0) {
    return mocks_not_initialized;
  }

  return mocks_success;
}
