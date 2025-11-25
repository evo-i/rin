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
title_read_from_file(FILE *file, struct title *self);

size_t
title_write_to_file(FILE *file, struct title const *self);

size_t
title_free(struct title *self);

size_t
title_hash(struct title const *self);

#ifdef __cplusplus
}
#endif

#endif
