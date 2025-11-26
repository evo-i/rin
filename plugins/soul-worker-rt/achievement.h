#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>

#include "prefixed_string.h"

struct achievement {
  uint32_t                id;
  struct prefixed_wstring title;
  struct prefixed_wstring description;
};

struct achievement *
achievement_new(void);

size_t
achievement_read(FILE *file, struct achievement *self);

size_t
achievement_write(FILE *file, struct achievement const *self);

void
achievement_free(struct achievement *self);

void
achievement_print(struct achievement const *self);

size_t
achievement_hash(struct achievement const *self);

size_t
achievement_size(struct achievement const *self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ACHIEVEMENT_H */
