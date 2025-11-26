#include "table.h"
#include "common.h"
#include "achievement.h"
#include "booster.h"
#include "buff.h"
#include "cinema.h"
#include "cutscene.h"
#include "item.h"
#include "league_skill.h"
#include "monster.h"
#include "npc.h"
#include "quest.h"
#include "shop.h"
#include "skill.h"
#include "soul_metry.h"
#include "speech.h"
#include "system_mail.h"
#include "title.h"
#include "tooltip.h"
#include "ui.h"
#include "md5.h"

#include <stdlib.h>
#include <stdio.h>

#define IS_VALID_TABLE_TYPE(type, arr) \
  ((type) != table_type_unknown && (type) < ARRAY_SIZE(arr) && (arr)[(type)] != NULL)

typedef size_t  (*item_read_t)  (FILE *file, void *item);
typedef size_t  (*item_write_t) (FILE *file, void const *item);
typedef size_t  (*item_size_t)  (void const *item);
typedef size_t  (*item_hash_t)  (void const *item);
typedef void    (*item_free_t)  (void *item);

static struct {
  size_t item_size;
  item_read_t read_func;
} item_read_info[] = {
  [table_type_none] = {
    0, NULL
  },
  [table_type_achievement] = {
    sizeof(struct achievement),
    (item_read_t) achievement_read
  },
  [table_type_booster] = {
    sizeof(struct booster),
    (item_read_t) booster_read
  },
  [table_type_buff] = {
    sizeof(struct buff),
    (item_read_t) buff_read
  },
  [table_type_cinema] = {
    sizeof(struct cinema),
    (item_read_t) cinema_read
  },
  [table_type_cutscene] = {
    sizeof(struct cutscene),
    (item_read_t) cutscene_read
  },
  [table_type_item] = {
    sizeof(struct item),
    (item_read_t) item_read
  },
  [table_type_league_skill] = {
    sizeof(struct league_skill),
    (item_read_t) league_skill_read
  },
  [table_type_monster] = {
    sizeof(struct monster),
    (item_read_t) monster_read
  },
  [table_type_npc] = {
    sizeof(struct npc),
    (item_read_t) npc_read
  },
  [table_type_quest] = {
    sizeof(struct quest),
    (item_read_t) quest_read
  },
  [table_type_shop] = {
    sizeof(struct shop),
    (item_read_t) shop_read
  },
  [table_type_skill] = {
    sizeof(struct skill),
    (item_read_t) skill_read
  },
  [table_type_soul_metry] = {
    sizeof(struct soul_metry),
    (item_read_t) soul_metry_read
  },
  [table_type_speech] = {
    sizeof(struct speech),
    (item_read_t) speech_read
  },
  [table_type_system_mail] = {
    sizeof(struct system_mail),
    (item_read_t) system_mail_read
  },
  [table_type_title] = {
    sizeof(struct title),
    (item_read_t) title_read
  },
  [table_type_tooltip] = {
    sizeof(struct tooltip),
    (item_read_t) tooltip_read
  },
  [table_type_ui] = {
    sizeof(struct ui),
    (item_read_t) ui_read
  }
};

static item_write_t item_write_cb[] = {
  [table_type_none]         = NULL,
  [table_type_achievement]  = (item_write_t) achievement_write,
  [table_type_booster]      = (item_write_t) booster_write,
  [table_type_buff]         = (item_write_t) buff_write,
  [table_type_cinema]       = (item_write_t) cinema_write,
  [table_type_cutscene]     = (item_write_t) cutscene_write,
  [table_type_item]         = (item_write_t) item_write,
  [table_type_league_skill] = (item_write_t) league_skill_write,
  [table_type_monster]      = (item_write_t) monster_write,
  [table_type_npc]          = (item_write_t) npc_write,
  [table_type_quest]        = (item_write_t) quest_write,
  [table_type_shop]         = (item_write_t) shop_write,
  [table_type_skill]        = (item_write_t) skill_write,
  [table_type_soul_metry]   = (item_write_t) soul_metry_write,
  [table_type_speech]       = (item_write_t) speech_write,
  [table_type_system_mail]  = (item_write_t) system_mail_write,
  [table_type_title]        = (item_write_t) title_write,
  [table_type_tooltip]      = (item_write_t) tooltip_write,
  [table_type_ui]           = (item_write_t) ui_write
};

static item_size_t item_size_cb[] = {
  [table_type_none]         = NULL,
  [table_type_achievement]  = (item_size_t) achievement_size,
  [table_type_booster]      = (item_size_t) booster_size,
  [table_type_buff]         = (item_size_t) buff_size,
  [table_type_cinema]       = (item_size_t) cinema_size,
  [table_type_cutscene]     = (item_size_t) cutscene_size,
  [table_type_item]         = (item_size_t) item_size,
  [table_type_league_skill] = (item_size_t) league_skill_size,
  [table_type_monster]      = (item_size_t) monster_size,
  [table_type_npc]          = (item_size_t) npc_size,
  [table_type_quest]        = (item_size_t) quest_size,
  [table_type_shop]         = (item_size_t) shop_size,
  [table_type_skill]        = (item_size_t) skill_size,
  [table_type_soul_metry]   = (item_size_t) soul_metry_size,
  [table_type_speech]       = (item_size_t) speech_size,
  [table_type_system_mail]  = (item_size_t) system_mail_size,
  [table_type_title]        = (item_size_t) title_size,
  [table_type_tooltip]      = (item_size_t) tooltip_size,
  [table_type_ui]           = (item_size_t) ui_size
};

static item_hash_t item_hash_cb[] = {
  [table_type_none]         = NULL,
  [table_type_achievement]  = (item_hash_t) achievement_hash,
  [table_type_booster]      = (item_hash_t) booster_hash,
  [table_type_buff]         = (item_hash_t) buff_hash,
  [table_type_cinema]       = (item_hash_t) cinema_hash,
  [table_type_cutscene]     = (item_hash_t) cutscene_hash,
  [table_type_item]         = (item_hash_t) item_hash,
  [table_type_league_skill] = (item_hash_t) league_skill_hash,
  [table_type_monster]      = (item_hash_t) monster_hash,
  [table_type_npc]          = (item_hash_t) npc_hash,
  [table_type_quest]        = (item_hash_t) quest_hash,
  [table_type_shop]         = (item_hash_t) shop_hash,
  [table_type_skill]        = (item_hash_t) skill_hash,
  [table_type_soul_metry]   = (item_hash_t) soul_metry_hash,
  [table_type_speech]       = (item_hash_t) speech_hash,
  [table_type_system_mail]  = (item_hash_t) system_mail_hash,
  [table_type_title]        = (item_hash_t) title_hash,
  [table_type_tooltip]      = (item_hash_t) tooltip_hash,
  [table_type_ui]           = (item_hash_t) ui_hash
};

static item_free_t item_free_cb[] = {
  [table_type_none]         = NULL,
  [table_type_achievement]  = (item_free_t) achievement_free,
  [table_type_booster]      = (item_free_t) booster_free,
  [table_type_buff]         = (item_free_t) buff_free,
  [table_type_cinema]       = (item_free_t) cinema_free,
  [table_type_cutscene]     = (item_free_t) cutscene_free,
  [table_type_item]         = (item_free_t) item_free,
  [table_type_league_skill] = (item_free_t) league_skill_free,
  [table_type_monster]      = (item_free_t) monster_free,
  [table_type_npc]          = (item_free_t) npc_free,
  [table_type_quest]        = (item_free_t) quest_free,
  [table_type_shop]         = (item_free_t) shop_free,
  [table_type_skill]        = (item_free_t) skill_free,
  [table_type_soul_metry]   = (item_free_t) soul_metry_free,
  [table_type_speech]       = (item_free_t) speech_free,
  [table_type_system_mail]  = (item_free_t) system_mail_free,
  [table_type_title]        = (item_free_t) title_free,
  [table_type_tooltip]      = (item_free_t) tooltip_free,
  [table_type_ui]           = (item_free_t) ui_free
};

static
size_t
table_read_count(FILE *file, uint32_t *count) {
  return fread(count, sizeof(*count), 1, file);
}

static
int
table_read_hash(FILE *file, struct prefixed_string *hash) {
  return prefixed_string_read(file, hash);
}

static
void **
read_items_generic(FILE *file, uint32_t count, size_t item_size, item_read_t read_func) {
  void **items = malloc(count * sizeof(void *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(item_size);
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (read_func(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return items;
}

struct table *
table_read(FILE *file, enum table_type table_type) {
  struct table *table
    = calloc(1, sizeof(struct table));

  if (!table) {
    return NULL;
  }

  table->type = table_type;

  if (table_read_count(file, &table->count) != 1) {
    table_free(table);
    return NULL;
  }

  if (table_type >= ARRAY_SIZE(item_read_info)
      || !item_read_info[table_type].read_func) {
    table_free(table);
    return NULL;
  }

  table->items = read_items_generic(file, table->count, 
                                    item_read_info[table_type].item_size,
                                    item_read_info[table_type].read_func);

  if (!table->items) {
    table_free(table);
    return NULL;
  }

  if (table_read_hash(file, &table->hash) == 0) {
    table_free(table);
    return NULL;
  }

  return table;
}

size_t
table_hash_num(struct table const *table) {
  size_t hash = 0;

  if (!table) {
    return 0;
  }

  if (!IS_VALID_TABLE_TYPE(table->type, item_hash_cb)) {
    return 0;
  }

  item_hash_t hash_func = item_hash_cb[table->type];

  for (uint32_t i = 0; i < table->count; i++) {
    hash += hash_func(table->items[i]);
  }

  return hash;
}

char const *
table_hash_string(struct table const *table) {
  static char hex_hash[33];
  size_t num_hash = table_hash_num(table);
  char num_str[64];
  uint8_t rgbHash[16];

  snprintf(num_str, sizeof(num_str), "%zu", num_hash);

  md5String(num_str, rgbHash);

  for (int i = 0; i < 16; i++) {
    snprintf(hex_hash + (i * 2), 3, "%02x", rgbHash[i]);
  }
  hex_hash[32] = 0;

  return hex_hash;
}

void
table_free(struct table *table) {
  if (!table) {
    return;
  }

  if (table->items && IS_VALID_TABLE_TYPE(table->type, item_free_cb)) {
    item_free_t free_func = item_free_cb[table->type];
    for (uint32_t i = 0; i < table->count; i++) {
      free_func(table->items[i]);
    }
    free(table->items);
  }

  prefixed_string_free(&table->hash);
  free(table);
}

size_t
table_write(FILE *file, struct table const *table) {
  if (!table) {
    return 0;
  }

  if (!IS_VALID_TABLE_TYPE(table->type, item_write_cb)) {
    return 0;
  }

  size_t total_size = 0;
  
  size_t written = fwrite(&table->count, sizeof(table->count), 1, file);
  if (written != 1) {
    return 0;
  }

  if (!safe_size_add(total_size, sizeof(table->count), &total_size)) {
    return 0;
  }

  item_write_t write_func = item_write_cb[table->type];
  for (uint32_t i = 0; i < table->count; i++) {
    size_t item_written = write_func(file, table->items[i]);
    if (item_written == 0) {
      return 0;
    }
    if (!safe_size_add(total_size, item_written, &total_size)) {
      return 0;
    }
  }

  size_t hash_written = prefixed_string_write(file, &table->hash);
  if (hash_written == 0) {
    return 0;
  }

  if (!safe_size_add(total_size, hash_written, &total_size)) {
    return 0;
  }

  return total_size;
}

size_t
table_size(struct table const *table) {
  if (!table) {
    return 0;
  }

  if (!IS_VALID_TABLE_TYPE(table->type, item_size_cb)) {
    return 0;
  }

  size_t size = sizeof(table->count);
  
  item_size_t size_func = item_size_cb[table->type];

  for (uint32_t i = 0; i < table->count; i++) {
    size += size_func(table->items[i]);
  }

  size += prefixed_string_size(&table->hash);

  return size;
}
