#ifndef BOOSTER_H_DEFINED
#define BOOSTER_H_DEFINED 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct booster {
  uint16_t                id;
  struct prefixed_wstring name;
  struct prefixed_wstring description;
  struct prefixed_wstring icon;
};

struct booster *
booster_new(void);

size_t
booster_read(FILE *file, struct booster *self);

size_t
booster_write(FILE *file, struct booster const *self);

void
booster_free(struct booster *self);

void
booster_print(struct booster const *self);

size_t
booster_hash(struct booster const *self);

size_t
booster_size(struct booster const *self);

#ifdef __cplusplus
}
#endif

#endif /* BOOSTER_H_DEFINED */
