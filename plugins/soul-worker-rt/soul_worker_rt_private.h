#ifndef SOUL_WORKER_RT_PRIVATE_H
#define SOUL_WORKER_RT_PRIVATE_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>

#include "prefixed_string.h"

#define LANGUAGE_PLACEHOLDER  "%language%"

#define LANGUAGE_ENGLISH      "ENG"
#define LANGUAGE_THAIWAN      "TWN"
#define LANGUAGE_CHINESE      "CHN"

#define TABLE_NAME(NAME, LANG) NAME "_" LANG ".res"

#define ACHIEVEMENT_TABLE_NAME \
  TABLE_NAME("tb_Achievement_Script", LANGUAGE_PLACEHOLDER)

#define BOOSTER_TABLE_NAME \
  TABLE_NAME("tb_Booster_Script", LANGUAGE_PLACEHOLDER)

#define BUFF_TABLE_NAME \
  TABLE_NAME("tb_Buff_Script", LANGUAGE_PLACEHOLDER)

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SOUL_WORKER_RT_PRIVATE_H */
