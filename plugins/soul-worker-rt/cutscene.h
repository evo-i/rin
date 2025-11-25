#ifndef CUTSCENE_H_DEFINED
#define CUTSCENE_H_DEFINED 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct cutscene {
  uint32_t id;
  struct prefixed_wstring text;
};

struct cutscene *
cutscene_new(void);

size_t
cutscene_read_from_file(FILE *file, struct cutscene *self);

size_t
cutscene_write_to_file(FILE *file, struct cutscene const *self);

size_t
cutscene_free(struct cutscene *self);

size_t
cutscene_print(struct cutscene const *self);

size_t
cutscene_compare(struct cutscene const *a, struct cutscene const *b);

size_t
cutscene_hash(struct cutscene const *self);

size_t
cutscene_size(struct cutscene const *self);

size_t
cutscene_copy(struct cutscene *dest, struct cutscene const *src);

#ifdef __cplusplus
}
#endif

#endif /* CUTSCENE_H_DEFINED */
