#ifndef TABLE_H
#define TABLE_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>

#include "prefixed_string.h"

enum table_type {
  table_type_none         = 0x0,
  table_type_achievement  = 0x1,
  table_type_booster      = 0x2,
  table_type_buff         = 0x3,
  table_type_cinema       = 0x4,
  table_type_cutscene     = 0x5,
  table_type_item         = 0x6,
  table_type_league_skill = 0x7,
  table_type_monster      = 0x8,
  table_type_npc          = 0x9,
  table_type_quest        = 0xA,
  table_type_shop         = 0xB,
  table_type_skill        = 0xC,
  table_type_soul_metry   = 0xD,
  table_type_speech       = 0xE,
  table_type_system_mail  = 0xF,
  table_type_title        = 0x10,
  table_type_tooltip      = 0x11,
  table_type_ui           = 0x12,
  table_type_unknown      = 0xFFFFFFFF
};

struct table {
  enum table_type        type;
  uint32_t                count;
  void                    **items;
  struct prefixed_string  hash;
};

struct table *
table_read(FILE *file, enum table_type table_type);

size_t
table_write(FILE *file, struct table const *table);

size_t
table_size(struct table const *table);

size_t
table_hash_num(struct table const *table);

char const *
table_hash_string(struct table const *table);

void
table_free(struct table *table);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TABLE_H */
