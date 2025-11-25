#ifndef TOOLTIP_H
#define TOOLTIP_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct tooltip {
  uint32_t                id;
  struct prefixed_wstring name;
  struct prefixed_wstring description;
};

struct tooltip *
tooltip_new(void);

size_t
tooltip_read_from_file(FILE *file, struct tooltip *self);

size_t
tooltip_write_to_file(FILE *file, struct tooltip const *self);

size_t
tooltip_free(struct tooltip *self);

size_t
tooltip_hash(struct tooltip const *self);

#ifdef __cplusplus
}
#endif

#endif
