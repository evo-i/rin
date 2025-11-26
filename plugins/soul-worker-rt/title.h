#ifndef TITLE_H
#define TITLE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct title {
  uint32_t                id;
  struct prefixed_wstring name;
  struct prefixed_wstring description;
  struct prefixed_wstring tip;
};

struct title *
title_new(void);

size_t
title_read(FILE *file, struct title *self);

size_t
title_write(FILE *file, struct title const *self);

void
title_free(struct title *self);

size_t
title_hash(struct title const *self);

size_t
title_size(struct title const *self);

#ifdef __cplusplus
}
#endif

#endif
