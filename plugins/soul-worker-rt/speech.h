#ifndef SPEECH_H
#define SPEECH_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct speech {
  uint32_t                id;
  struct prefixed_wstring text;
};

struct speech *
speech_new(void);

size_t
speech_read_from_file(FILE *file, struct speech *self);

size_t
speech_write_to_file(FILE *file, struct speech const *self);

size_t
speech_free(struct speech *self);

size_t
speech_hash(struct speech const *self);

#ifdef __cplusplus
}
#endif

#endif
