#ifndef MONSTER_H
#define MONSTER_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct monster {
  uint32_t                id;
  struct prefixed_wstring name;
};

struct monster *
monster_new(void);

size_t
monster_read(FILE *file, struct monster *self);

size_t
monster_write(FILE *file, struct monster const *self);

void
monster_free(struct monster *self);

size_t
monster_hash(struct monster const *self);

void
monster_print(struct monster const *self);

size_t
monster_size(struct monster const *self);

#ifdef __cplusplus
}
#endif

#endif
