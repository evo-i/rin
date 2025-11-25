#ifndef BUFF_H
#define BUFF_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct buff {
  uint32_t                id;
  struct prefixed_wstring name;
  struct prefixed_wstring description;
  struct prefixed_wstring icon;
  struct prefixed_wstring tip;
};

struct buff *
buff_new(void);

size_t
buff_read_from_file(FILE *file, struct buff *self);

size_t
buff_write_to_file(FILE *file, struct buff const *self);

size_t
buff_free(struct buff *self);

size_t
buff_print(struct buff const *self);

size_t
buff_compare(struct buff const *a,
             struct buff const *b);

size_t
buff_hash(struct buff const *self);

size_t
buff_size(struct buff const *self);

size_t
buff_copy(struct buff *dest,
          struct buff const *src);

#ifdef __cplusplus
}
#endif

#endif /* BUFF_H */
