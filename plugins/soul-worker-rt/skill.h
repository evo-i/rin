#ifndef SKILL_H
#define SKILL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct skill {
  uint32_t                id;
  struct prefixed_wstring name;
  struct prefixed_wstring info;
  struct prefixed_wstring buff;
  struct prefixed_wstring icon;
  uint32_t                unknown1;
  uint32_t                unknown2;
  struct prefixed_wstring description_image;
  struct prefixed_wstring movie;
};

struct skill *
skill_new(void);

size_t
skill_read(FILE *file, struct skill *self);

size_t
skill_write(FILE *file, struct skill const *self);

void
skill_free(struct skill *self);

size_t
skill_hash(struct skill const *self);

void
skill_print(struct skill const *self);

size_t
skill_size(struct skill const *self);

#ifdef __cplusplus
}
#endif

#endif
