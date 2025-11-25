#include "table.h"
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

#include <stdlib.h>
#include <stdio.h>
#include "md5.h"

typedef size_t (*cb_item_hash_func)(void const *item);

static
size_t
table_read_count(FILE *file, uint32_t *count) {
  return fread(count, sizeof(*count), 1, file);
}

static
int
table_read_hash(FILE *file, struct prefixed_string *hash) {
  return prefixed_string_read_from_file(file, hash);
}

static
void **
achievements_read_items(FILE *file, uint32_t count) {
  struct achievement **items
    = malloc(count * sizeof(struct achievement *));

  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct achievement));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (achievement_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
boosters_read_items(FILE *file, uint32_t count) {
  struct booster **items = malloc(count * sizeof(struct booster *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct booster));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (booster_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
buffs_read_items(FILE *file, uint32_t count) {
  struct buff **items = malloc(count * sizeof(struct buff *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct buff));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (buff_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
cinemas_read_items(FILE *file, uint32_t count) {
  struct cinema **items = malloc(count * sizeof(struct cinema *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct cinema));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (cinema_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
cutscenes_read_items(FILE *file, uint32_t count) {
  struct cutscene **items = malloc(count * sizeof(struct cutscene *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct cutscene));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (cutscene_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
items_read_items(FILE *file, uint32_t count) {
  struct item **items = malloc(count * sizeof(struct item *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct item));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (item_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
league_skills_read_items(FILE *file, uint32_t count) {
  struct league_skill **items = malloc(count * sizeof(struct league_skill *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct league_skill));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (league_skill_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
monsters_read_items(FILE *file, uint32_t count) {
  struct monster **items = malloc(count * sizeof(struct monster *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct monster));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (monster_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
npcs_read_items(FILE *file, uint32_t count) {
  struct npc **items = malloc(count * sizeof(struct npc *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct npc));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (npc_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
quests_read_items(FILE *file, uint32_t count) {
  struct quest **items = malloc(count * sizeof(struct quest *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct quest));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (quest_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
shops_read_items(FILE *file, uint32_t count) {
  struct shop **items = malloc(count * sizeof(struct shop *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct shop));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (shop_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
skills_read_items(FILE *file, uint32_t count) {
  struct skill **items = malloc(count * sizeof(struct skill *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct skill));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (skill_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
soul_metries_read_items(FILE *file, uint32_t count) {
  struct soul_metry **items = malloc(count * sizeof(struct soul_metry *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct soul_metry));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (soul_metry_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
speeches_read_items(FILE *file, uint32_t count) {
  struct speech **items = malloc(count * sizeof(struct speech *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct speech));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (speech_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
system_mails_read_items(FILE *file, uint32_t count) {
  struct system_mail **items = malloc(count * sizeof(struct system_mail *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct system_mail));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (system_mail_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
titles_read_items(FILE *file, uint32_t count) {
  struct title **items = malloc(count * sizeof(struct title *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct title));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (title_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
tooltips_read_items(FILE *file, uint32_t count) {
  struct tooltip **items = malloc(count * sizeof(struct tooltip *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct tooltip));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (tooltip_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

static
void **
uis_read_items(FILE *file, uint32_t count) {
  struct ui **items = malloc(count * sizeof(struct ui *));
  if (!items) {
    return NULL;
  }

  for (uint32_t i = 0; i < count; i++) {
    items[i] = malloc(sizeof(struct ui));
    if (!items[i]) {
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }

    if (ui_read_from_file(file, items[i]) == 0) {
      free(items[i]);
      for (uint32_t j = 0; j < i; j++) {
        free(items[j]);
      }
      free(items);
      return NULL;
    }
  }

  return (void **) items;
}

struct table *
table_read_from_file(FILE *file, enum table_type table_type) {
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

  switch (table_type) {
    case table_type_achievement: {
      table->items = achievements_read_items(file, table->count);
    } break;
    case table_type_booster: {
      table->items = boosters_read_items(file, table->count);
    } break;
    case table_type_buff: {
      table->items = buffs_read_items(file, table->count);
    } break;
    case table_type_cinema: {
      table->items = cinemas_read_items(file, table->count);
    } break;
    case table_type_cutscene: {
      table->items = cutscenes_read_items(file, table->count);
    } break;
    case table_type_item: {
      table->items = items_read_items(file, table->count);
    } break;
    case table_type_league_skill: {
      table->items = league_skills_read_items(file, table->count);
    } break;
    case table_type_monster: {
      table->items = monsters_read_items(file, table->count);
    } break;
    case table_type_npc: {
      table->items = npcs_read_items(file, table->count);
    } break;
    case table_type_quest: {
      table->items = quests_read_items(file, table->count);
    } break;
    case table_type_shop: {
      table->items = shops_read_items(file, table->count);
    } break;
    case table_type_skill: {
      table->items = skills_read_items(file, table->count);
    } break;
    case table_type_soul_metry: {
      table->items = soul_metries_read_items(file, table->count);
    } break;
    case table_type_speech: {
      table->items = speeches_read_items(file, table->count);
    } break;
    case table_type_system_mail: {
      table->items = system_mails_read_items(file, table->count);
    } break;
    case table_type_title: {
      table->items = titles_read_items(file, table->count);
    } break;
    case table_type_tooltip: {
      table->items = tooltips_read_items(file, table->count);
    } break;
    case table_type_ui: {
      table->items = uis_read_items(file, table->count);
    } break;
    default: {
      table_free(table);
      return NULL;
    } break;
  }

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

  cb_item_hash_func hash_func = NULL;

  switch (table->type) {
    case table_type_achievement:
      hash_func = (cb_item_hash_func) achievement_hash;
      break;
    case table_type_booster:
      hash_func = (cb_item_hash_func) booster_hash;
      break;
    case table_type_buff:
      hash_func = (cb_item_hash_func) buff_hash;
      break;
    case table_type_cinema:
      hash_func = (cb_item_hash_func) cinema_hash;
      break;
    case table_type_cutscene:
      hash_func = (cb_item_hash_func) cutscene_hash;
      break;
    case table_type_item:
      hash_func = (cb_item_hash_func) item_hash;
      break;
    case table_type_league_skill:
      hash_func = (cb_item_hash_func) league_skill_hash;
      break;
    case table_type_monster:
      hash_func = (cb_item_hash_func) monster_hash;
      break;
    case table_type_npc:
      hash_func = (cb_item_hash_func) npc_hash;
      break;
    case table_type_quest:
      hash_func = (cb_item_hash_func) quest_hash;
      break;
    case table_type_shop:
      hash_func = (cb_item_hash_func) shop_hash;
      break;
    case table_type_skill:
      hash_func = (cb_item_hash_func) skill_hash;
      break;
    case table_type_soul_metry:
      hash_func = (cb_item_hash_func) soul_metry_hash;
      break;
    case table_type_speech:
      hash_func = (cb_item_hash_func) speech_hash;
      break;
    case table_type_system_mail:
      hash_func = (cb_item_hash_func) system_mail_hash;
      break;
    case table_type_title:
      hash_func = (cb_item_hash_func) title_hash;
      break;
    case table_type_tooltip:
      hash_func = (cb_item_hash_func) tooltip_hash;
      break;
    case table_type_ui:
      hash_func = (cb_item_hash_func) ui_hash;
      break;
    default:
      return 0;
  }

  if (!hash_func) {
    return 0;
  }

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

  if (table->items) {
    switch (table->type) {
      case table_type_achievement: {
        for (uint32_t i = 0; i < table->count; i++) {
          achievement_free(table->items[i]);
        }
      } break;
      case table_type_booster: {
        for (uint32_t i = 0; i < table->count; i++) {
          booster_free(table->items[i]);
        }
      } break;
      case table_type_buff: {
        for (uint32_t i = 0; i < table->count; i++) {
          buff_free(table->items[i]);
        }
      } break;
      case table_type_cinema: {
        for (uint32_t i = 0; i < table->count; i++) {
          cinema_free(table->items[i]);
        }
      } break;
      case table_type_cutscene: {
        for (uint32_t i = 0; i < table->count; i++) {
          cutscene_free(table->items[i]);
        }
      } break;
      case table_type_item: {
        for (uint32_t i = 0; i < table->count; i++) {
          item_free(table->items[i]);
        }
      } break;
      case table_type_league_skill: {
        for (uint32_t i = 0; i < table->count; i++) {
          league_skill_free(table->items[i]);
        }
      } break;
      case table_type_monster: {
        for (uint32_t i = 0; i < table->count; i++) {
          monster_free(table->items[i]);
        }
      } break;
      case table_type_npc: {
        for (uint32_t i = 0; i < table->count; i++) {
          npc_free(table->items[i]);
        }
      } break;
      case table_type_quest: {
        for (uint32_t i = 0; i < table->count; i++) {
          quest_free(table->items[i]);
        }
      } break;
      case table_type_shop: {
        for (uint32_t i = 0; i < table->count; i++) {
          shop_free(table->items[i]);
        }
      } break;
      case table_type_skill: {
        for (uint32_t i = 0; i < table->count; i++) {
          skill_free(table->items[i]);
        }
      } break;
      case table_type_soul_metry: {
        for (uint32_t i = 0; i < table->count; i++) {
          soul_metry_free(table->items[i]);
        }
      } break;
      case table_type_speech: {
        for (uint32_t i = 0; i < table->count; i++) {
          speech_free(table->items[i]);
        }
      } break;
      case table_type_system_mail: {
        for (uint32_t i = 0; i < table->count; i++) {
          system_mail_free(table->items[i]);
        }
      } break;
      case table_type_title: {
        for (uint32_t i = 0; i < table->count; i++) {
          title_free(table->items[i]);
        }
      } break;
      case table_type_tooltip: {
        for (uint32_t i = 0; i < table->count; i++) {
          tooltip_free(table->items[i]);
        }
      } break;
      case table_type_ui: {
        for (uint32_t i = 0; i < table->count; i++) {
          ui_free(table->items[i]);
        }
      } break;
      default:
        break;
    }
    free(table->items);
  }

  prefixed_string_free(&table->hash);
  free(table);
}
