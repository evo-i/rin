#ifndef ITEM_H_DEFINED
#define ITEM_H_DEFINED 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct item {
  uint32_t                id;
  struct prefixed_wstring unknown1;
  struct prefixed_wstring unknown2;
  struct prefixed_wstring unknown3;
  struct prefixed_wstring unknown4;
  struct prefixed_wstring unknown5;
  struct prefixed_wstring unknown6;
  uint8_t                 flags[5];
  struct prefixed_wstring name;
  struct prefixed_wstring description;
};

struct item *
item_new(void);

size_t
item_read_from_file(FILE *file, struct item *self);

size_t
item_write_to_file(FILE *file, struct item const *self);

size_t
item_free(struct item *self);

size_t
item_print(struct item const *self);

size_t
item_compare(struct item const *a,
             struct item const *b);

size_t
item_hash(struct item const *self);

size_t
item_size(struct item const *self);

size_t
item_copy(struct item *dest,
          struct item const *src);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ITEM_H_DEFINED */
