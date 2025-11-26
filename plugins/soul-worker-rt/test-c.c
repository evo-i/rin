#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

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

#ifdef _WIN32
#define chdir _chdir
#define getcwd _getcwd
#endif

#define TABLE_PATH_PREFIX \
"F:\\msys64\\home\\devbi\\rin\\build\\packages\\swae-1.0.0-win64\\bin\\extracted\\data11\\bin\\Table\\"

#define ACHIEVEMENTS_TABLE "tb_Achievement_Script_ENG.res"
#define BOOSTER_TABLE      "tb_Booster_Script_ENG.res"
#define BUFF_TABLE         "tb_Buff_Script_ENG.res"
#define CINEMA_TABLE       "tb_Cinema_String_ENG.res"
#define CUTSCENE_TABLE     "tb_Cutscene_String_ENG.res"
#define ITEM_TABLE         "tb_Item_Script_ENG.res"
#define UI_TABLE           "tb_UI_String_ENG.res"
#define LEAGUE_SKILL_TABLE "tb_LeagueSkill_Script_ENG.res"
#define MONSTER_TABLE      "tb_Monster_Script_ENG.res"
#define NPC_TABLE          "tb_Npc_Script_ENG.res"
#define QUEST_TABLE        "tb_Quest_Script_ENG.res"
#define SHOP_TABLE         "tb_Shop_String_ENG.res"
#define SKILL_TABLE        "tb_Skill_Script_ENG.res"
#define SOUL_METRY_TABLE   "tb_soul_metry_string_ENG.res"
#define SPEECH_TABLE       "tb_Speech_String_ENG.res"
#define SYSTEM_MAIL_TABLE  "tb_SystemMail_ENG.res"
#define TITLE_TABLE        "tb_Title_String_ENG.res"
#define TOOLTIP_TABLE      "tb_Tooltip_String_ENG.res"

static const char*
get_table_name(enum table_type type) {
  switch (type) {
    case table_type_achievement: return "Achievement";
    case table_type_booster: return "Booster";
    case table_type_buff: return "Buff";
    case table_type_cinema: return "Cinema";
    case table_type_cutscene: return "Cutscene";
    case table_type_item: return "Item";
    case table_type_league_skill: return "League Skill";
    case table_type_monster: return "Monster";
    case table_type_npc: return "NPC";
    case table_type_quest: return "Quest";
    case table_type_shop: return "Shop";
    case table_type_skill: return "Skill";
    case table_type_soul_metry: return "Soul Metry";
    case table_type_speech: return "Speech";
    case table_type_system_mail: return "System Mail";
    case table_type_title: return "Title";
    case table_type_tooltip: return "Tooltip";
    case table_type_ui: return "UI";
    default: return "Unknown";
  }
}

static void
print_table_header() {
  printf("+----------------------+----------------------------------+----------------------------------+--------+\n");
  printf("| %-20s | %-32s | %-32s | %-6s |\n", "Table Name", "File Hash", "Calculated Hash", "Status");
  printf("+----------------------+----------------------------------+----------------------------------+--------+\n");
}

static void
print_table_footer() {
  printf("+----------------------+----------------------------------+----------------------------------+--------+\n");
}

static
void
table_info(struct table const *table, enum table_type type) {
  const char* name = get_table_name(type);

  if (!table) {
    printf("| %-20s | %-32s | %-32s | %-6s |\n", name, "N/A", "N/A", "FAIL");
    return;
  }
  
  char const *calc_hash = table_hash_string(table);
  const char *status = (strcmp(table->hash.value, calc_hash) == 0) ? "OK" : "BAD";

  printf("| %-20s | %-32s | %-32s | %-6s |\n", name, table->hash.value, calc_hash, status);
}

static
void
test_table_file(const char* table_name, enum table_type type) {
  char *cwd;
  int status;
  struct table *table;
  
  cwd = getcwd(NULL, 0);
  status = chdir(TABLE_PATH_PREFIX);

  if (status != 0) {
    fprintf(stderr, "Failed to change directory to %s\n", TABLE_PATH_PREFIX);
    free(cwd);
    return;
  }

  FILE *table_file = fopen(table_name, "rb");

  if (!table_file) {
    fprintf(stderr, "Failed to open table file: %s reason: %s\n", table_name, strerror(errno));
  }

  if (table_file) {
    table = table_read(table_file, type);
    fclose(table_file);
  } else {
    table = NULL;
  }

  table_info(table, type);

  if (table) {
    table_free(table);
  }

  chdir(cwd);
  free(cwd);
}

int
main(int argc, char* argv[]) {
  // print_table_header();

  // test_table_file(ACHIEVEMENTS_TABLE, table_type_achievement);
  // test_table_file(BOOSTER_TABLE,      table_type_booster);
  // test_table_file(BUFF_TABLE,         table_type_buff);
  // test_table_file(CINEMA_TABLE,       table_type_cinema);
  // test_table_file(CUTSCENE_TABLE,     table_type_cutscene);
  // test_table_file(ITEM_TABLE,         table_type_item);
  // test_table_file(LEAGUE_SKILL_TABLE, table_type_league_skill);
  // test_table_file(MONSTER_TABLE,      table_type_monster);
  // test_table_file(NPC_TABLE,          table_type_npc);
  // test_table_file(QUEST_TABLE,        table_type_quest);
  // test_table_file(SHOP_TABLE,         table_type_shop);
  // test_table_file(SKILL_TABLE,        table_type_skill);
  // test_table_file(SOUL_METRY_TABLE,   table_type_soul_metry);
  // test_table_file(SPEECH_TABLE,       table_type_speech);
  // test_table_file(SYSTEM_MAIL_TABLE,  table_type_system_mail);
  // test_table_file(TITLE_TABLE,        table_type_title);
  // test_table_file(TOOLTIP_TABLE,      table_type_tooltip);
  // test_table_file(UI_TABLE,           table_type_ui);

  // print_table_footer();

  FILE *ui_file
    = fopen(TABLE_PATH_PREFIX UI_TABLE, "rb");

  if (ui_file) {
    struct table *ui_table
      = table_read(
          ui_file,
          table_type_ui);
    if (ui_table) {
      for (uint32_t i = 0; i < ui_table->count; i++) {
        struct ui *ui = ui_table->items[i];

        // if (ui->unknown1 != 0)
          ui_print(ui);
      }
    }

    table_free(ui_table);
    fclose(ui_file);
  }

  return 0;
}
