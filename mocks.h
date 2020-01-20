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
} mocks_return_code;


mocks_return_code
mocks_init (uint32_t number_of_threads, uint32_t context_buffer_size);

mocks_return_code
mocks_init_thread (
  uint32_t    thread_index,
  pthread_t  *thread_id,
  uint32_t    number_of_expectations);

mocks_return_code
mocks_verify (void);


#endif /* MOCKS_H */
