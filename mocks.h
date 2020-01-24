#ifndef MOCKS_H
#define MOCKS_H

#include <stdint.h>
#include <pthread.h>


typedef enum {
  mocks_success,
  mocks_not_initialized,
  mocks_thread_index_out_of_range,
  mocks_thread_id_is_null,
  mocks_thread_bad_number_of_expectations,
  mocks_no_more_expectations,
  mocks_no_room_for_expectation,
  mocks_invalid_ctx_size,
  mocks_invalid_ctx,
} mocks_return_code;

typedef struct {
  int         id;
  int         context_size;
  void       *context_data;
} mocks_expectation_t;


mocks_return_code
mocks_init (int number_of_threads, int context_buffer_size);

mocks_return_code
mocks_init_thread (
  int         thread_index,
  pthread_t  *thread_id,
  int         number_of_expectations);

void
mocks_cleanup (void);

mocks_return_code
mocks_expect (
  int                         thread_index,
  const mocks_expectation_t  *expectation);

mocks_return_code
mocks_invoke(
  int        *expectation_id,
  int        *context_size,
  void      **context_data);

mocks_return_code
mocks_verify (void);


#endif /* MOCKS_H */
