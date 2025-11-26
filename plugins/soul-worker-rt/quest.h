#ifndef QUEST_H
#define QUEST_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct quest {
  uint32_t                id;
  struct prefixed_wstring title;
  struct prefixed_wstring text;
  struct prefixed_wstring task;
  struct prefixed_wstring talk;
  struct prefixed_wstring destination;
};

struct quest *
quest_new(void);

size_t
quest_read(FILE *file, struct quest *self);

size_t
quest_write(FILE *file, struct quest const *self);

void
quest_free(struct quest *self);

size_t
quest_hash(struct quest const *self);

size_t
quest_size(struct quest const *self);

#ifdef __cplusplus
}
#endif

#endif
