#ifndef UI_H
#define UI_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct ui {
  uint32_t                id;
  uint8_t                 has_format;
  struct prefixed_wstring text;
};

struct ui *
ui_new(void);

size_t
ui_read_from_file(FILE *file, struct ui *self);

size_t
ui_write_to_file(FILE *file, struct ui const *self);

size_t
ui_free(struct ui *self);

size_t
ui_hash(struct ui const *self);

#ifdef __cplusplus
}
#endif

#endif
