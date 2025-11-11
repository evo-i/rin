#ifndef SOUL_WORKER_RT_PRIVATE_H
#define SOUL_WORKER_RT_PRIVATE_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

typedef struct prefixed_string_s {
  uint16_t  length;
  char* value;
} prefixed_string_t;

typedef struct prefixed_wstring_s {
  uint16_t  length;
  uint16_t* value;
} prefixed_wstring_t;

typedef struct achievement_data_s {
  uint32_t          id;
  prefixed_wstring_t title;
  prefixed_wstring_t description;
} achievement_data_t;

typedef struct achievement_table_s {
  uint32_t            count;
  achievement_data_t* achievements;
  prefixed_string_t   hash;
} achievement_table_t;

typedef struct booster_data_s {
  uint16_t          id;
  prefixed_string_t name;
  prefixed_string_t description;
  prefixed_string_t icon;
} booster_data_t;

typedef struct booster_table_s {
  uint32_t        count;
  booster_data_t* boosters;
  uint8_t*        hash;
} booster_table_t;

typedef struct buff_data_s {
  uint16_t          id;
  prefixed_string_t name;
  prefixed_string_t description;
  prefixed_string_t icon;
  prefixed_string_t effect;
} buff_data_t;

typedef struct buff_table_s {
  uint32_t     count;
  buff_data_t* buffs;
  uint8_t*     hash;
} buff_table_t;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SOUL_WORKER_RT_PRIVATE_H */
