#ifndef LEAGUE_SKILL_H
#define LEAGUE_SKILL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct league_skill {
  uint16_t                id;
  struct prefixed_wstring icon;
  struct prefixed_wstring name;
  struct prefixed_wstring unknown;
  struct prefixed_wstring description;
  struct prefixed_wstring title;
};

struct league_skill *
league_skill_new(void);

size_t
league_skill_read_from_file(FILE *file, struct league_skill *self);

size_t
league_skill_write_to_file(FILE *file, struct league_skill const *self);

size_t
league_skill_free(struct league_skill *self);

size_t
league_skill_hash(struct league_skill const *self);

#ifdef __cplusplus
}
#endif

#endif
