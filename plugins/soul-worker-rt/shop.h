#ifndef SHOP_H
#define SHOP_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct shop {
  uint32_t                id;
  struct prefixed_wstring name;
};

struct shop *
shop_new(void);

size_t
shop_read(FILE *file, struct shop *self);

size_t
shop_write(FILE *file, struct shop const *self);

void
shop_free(struct shop *self);

size_t
shop_hash(struct shop const *self);

size_t
shop_size(struct shop const *self);

#ifdef __cplusplus
}
#endif

#endif
