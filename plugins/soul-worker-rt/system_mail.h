#ifndef SYSTEM_MAIL_H
#define SYSTEM_MAIL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct system_mail {
  uint8_t                 id;
  uint16_t                unknown1;
  uint16_t                unknown2;
  struct prefixed_wstring from;
  struct prefixed_wstring subject;
  struct prefixed_wstring text;
  struct prefixed_wstring image;
};

struct system_mail *
system_mail_new(void);

size_t
system_mail_read(FILE *file, struct system_mail *self);

size_t
system_mail_write(FILE *file, struct system_mail const *self);

void
system_mail_free(struct system_mail *self);

size_t
system_mail_hash(struct system_mail const *self);

size_t
system_mail_size(struct system_mail const *self);

void
system_mail_print(struct system_mail const *self);

#ifdef __cplusplus
}
#endif

#endif
