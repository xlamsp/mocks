#ifndef MOCKS_H
#define MOCKS_H

#include <stdint.h>


typedef enum {
  mocks_success,
  mocks_not_initialized,
} mocks_return_code;


mocks_return_code
mocks_init (uint32_t number_of_threads, uint32_t context_buffer_size);

mocks_return_code
mocks_verify (void);


#endif /* MOCKS_H */
