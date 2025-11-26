#ifndef ITEM_H_DEFINED
#define ITEM_H_DEFINED 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>

#include "prefixed_string.h"

#define ITEM_RESOURCE_COUNT 5

struct item_resources {
  struct prefixed_wstring values[ITEM_RESOURCE_COUNT];
  uint8_t                 flags[ITEM_RESOURCE_COUNT];
};

struct item {
  uint32_t                id;
  struct prefixed_wstring icon;
  struct item_resources   resources;
  struct prefixed_wstring name;
  struct prefixed_wstring description;
};

struct item *
item_new(void);

size_t
item_read(FILE *file, struct item *self);

size_t
item_write(FILE *file, struct item const *self);

void
item_free(struct item *self);

size_t
item_print(struct item const *self);

size_t
item_hash(struct item const *self);

size_t
item_size(struct item const *self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ITEM_H_DEFINED */
