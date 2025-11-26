#ifndef SOUL_METRY_H
#define SOUL_METRY_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct soul_metry {
  uint16_t                id;
  struct prefixed_wstring text;
};

struct soul_metry *
soul_metry_new(void);

size_t
soul_metry_read(FILE *file, struct soul_metry *self);

size_t
soul_metry_write(FILE *file, struct soul_metry const *self);

void
soul_metry_free(struct soul_metry *self);

size_t
soul_metry_hash(struct soul_metry const *self);

size_t
soul_metry_size(struct soul_metry const *self);

#ifdef __cplusplus
}
#endif

#endif
