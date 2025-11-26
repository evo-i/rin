#ifndef CINEMA_H_DEFINED
#define CINEMA_H_DEFINED 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

#define CINEMA_STRING_COUNT 12

struct cinema {
  uint16_t id;
  struct prefixed_wstring strings[CINEMA_STRING_COUNT];
};

struct cinema *
cinema_new(void);

size_t
cinema_read(FILE *file, struct cinema *self);

size_t
cinema_write(FILE *file, struct cinema const *self);

void
cinema_free(struct cinema *self);

void
cinema_print(struct cinema const *self);

size_t
cinema_hash(struct cinema const *self);

size_t
cinema_size(struct cinema const *self);

#ifdef __cplusplus
}
#endif

#endif /* CINEMA_H_DEFINED */
