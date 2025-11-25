#ifndef PREFIXED_STRING_H_DEFINED
#define PREFIXED_STRING_H_DEFINED 1

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct prefixed_string {
  uint16_t  length;
  char      *value;
};

struct prefixed_wstring {
  uint16_t  length;
  uint16_t  *value;
};

static inline
size_t
prefixed_string_size(const struct prefixed_string* str) {
  if (!str) {
    return 0;
  }

  return
    sizeof(str->length)
      + (str->length * sizeof(str->value[0]));
}

static inline
size_t
prefixed_wstring_size(const struct prefixed_wstring* str) {
  if (!str) {
    return 0;
  }
  
  return
    sizeof(str->length)
      + (str->length * sizeof(str->value[0]));
}

size_t
prefixed_string_read_from_file(FILE *file, struct prefixed_string *self);

size_t
prefixed_wstring_read_from_file(FILE *file, struct prefixed_wstring *self);

size_t
prefixed_string_write_to_file(FILE *file, struct prefixed_string const *self);

size_t
prefixed_wstring_write_to_file(FILE *file, struct prefixed_wstring const *self);

size_t
prefixed_string_skip_in_file(FILE *file);

size_t
prefixed_wstring_skip_in_file(FILE *file);

size_t
prefixed_string_copy(struct prefixed_string *dest,
                     struct prefixed_string const *src);

size_t
prefixed_wstring_copy(struct prefixed_wstring *dest,
                      struct prefixed_wstring const *src);

void
prefixed_string_free(struct prefixed_string *self);

void
prefixed_wstring_free(struct prefixed_wstring *self);

size_t
prefixed_string_print(struct prefixed_string const *self);

size_t
prefixed_wstring_print(struct prefixed_wstring const *self);

size_t
prefixed_string_compare(struct prefixed_string const *a,
                        struct prefixed_string const *b);

size_t
prefixed_wstring_compare(struct prefixed_wstring const *a,
                         struct prefixed_wstring const *b);

size_t
prefixed_string_hash(struct prefixed_string const *self);

size_t
prefixed_wstring_hash(struct prefixed_wstring const *self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PREFIXED_STRING_H_DEFINED */
