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
cutscene_read(FILE *file, struct cutscene *self);

size_t
cutscene_write(FILE *file, struct cutscene const *self);

void
cutscene_free(struct cutscene *self);

void
cutscene_print(struct cutscene const *self);

size_t
cutscene_hash(struct cutscene const *self);

size_t
cutscene_size(struct cutscene const *self);


#ifdef __cplusplus
}
#endif

#endif /* CUTSCENE_H_DEFINED */
