#include "prefixed_string.h"
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

size_t
prefixed_string_write_to_file(FILE *file, struct prefixed_string const *self) {
  if (!self) {
    return 0;
  }

  size_t write_count = 0;

  write_count = fwrite(&self->length, sizeof(self->length), 1, file);

  if (write_count != 1) {
    return 0;
  }

  if (self->length > 0 && self->value) {
    size_t value_write = fwrite(self->value, sizeof(self->value[0]), self->length, file);

    if (value_write != self->length) {
      return 0;
    }

    write_count += value_write;
  }

  return write_count;
}

size_t
prefixed_wstring_write_to_file(FILE *file, struct prefixed_wstring const *self) {
  if (!self) {
    return 0;
  }

  size_t write_count = 0;

  write_count = fwrite(&self->length, sizeof(self->length), 1, file);

  if (write_count != 1) {
    return 0;
  }

  if (self->length > 0 && self->value) {
    size_t value_write = fwrite(self->value, sizeof(self->value[0]), self->length, file);

    if (value_write != self->length) {
      return 0;
    }

    write_count += value_write;
  }

  return write_count;
}

size_t
prefixed_string_skip_in_file(FILE *file) {
  uint16_t length = 0;
  size_t   read_count =
    fread(&length, sizeof(length), 1, file);

  if (read_count != 1) {
    return 0;
  }

  if (length > 0) {
    if (fseek(file, length * sizeof(char), SEEK_CUR) != 0) {
      return 0;
    }
  }

  return sizeof(length) + (length * sizeof(char));
}

size_t
prefixed_wstring_skip_in_file(FILE *file) {
  uint16_t length = 0;
  size_t   read_count =
    fread(&length, sizeof(length), 1, file);

  if (read_count != 1) {
    return 0;
  }

  if (length > 0) {
    if (fseek(file, length * sizeof(uint16_t), SEEK_CUR) != 0) {
      return 0;
    }
  }

  return sizeof(length) + (length * sizeof(uint16_t));
}

size_t
prefixed_string_copy(struct prefixed_string *dest,
                     struct prefixed_string const *src) {
  if (!dest || !src) {
    return 0;
  }

  dest->length = src->length;

  if (src->length > 0 && src->value) {
    dest->value = malloc((src->length + 1) * sizeof(char));
    if (!dest->value) {
      dest->length = 0;
      return 0;
    }

    memcpy(dest->value, src->value, src->length * sizeof(char));
    dest->value[src->length] = '\0';
  } else {
    dest->value = NULL;
  }

  return prefixed_string_size(dest);
}

size_t
prefixed_wstring_copy(struct prefixed_wstring *dest,
                      struct prefixed_wstring const *src) {
  if (!dest || !src) {
    return 0;
  }

  dest->length = src->length;

  if (src->length > 0 && src->value) {
    dest->value = malloc((src->length + 1) * sizeof(uint16_t));
    if (!dest->value) {
      dest->length = 0;
      return 0;
    }

    memcpy(dest->value, src->value, src->length * sizeof(uint16_t));
    dest->value[src->length] = 0;
  } else {
    dest->value = NULL;
  }

  return prefixed_wstring_size(dest);
}

size_t
prefixed_string_read_from_file(FILE *file, struct prefixed_string *self) {
  size_t read_count = 0;
  *self = (struct prefixed_string) { 0 };

  read_count = fread(&self->length, sizeof(self->length), 1, file);

  if (read_count != 1) {
    return 0;
  }

  if (self->length > 0) {
    self->value = malloc((self->length + 1) * sizeof(self->value[0]));
    if (!self->value) {
      return 0;
    }

    read_count = fread(self->value, sizeof(self->value[0]), self->length, file);
    if (read_count != self->length) {
      free(self->value);
      self->value = NULL;
      return 0;
    }

    self->value[self->length] = 0;
  }

  return
    sizeof(self->length)
      + (self->length * sizeof(self->value[0]));
}

size_t
prefixed_wstring_read_from_file(FILE *file, struct prefixed_wstring *self) {
  size_t read_count = 0;
  *self = (struct prefixed_wstring) { 0 };

  read_count = fread(&self->length, sizeof(self->length), 1, file);

  if (read_count != 1) {
    return 0;
  }

  if (self->length > 0) {
    self->value = malloc((self->length + 1) * sizeof(self->value[0]));
    if (!self->value) {
      return 0;
    }

    read_count = fread(self->value, sizeof(self->value[0]), self->length, file);
    if (read_count != self->length) {
      free(self->value);
      self->value = NULL;
      return 0;
    }

    self->value[self->length] = 0;
  }

  return
    sizeof(self->length)
      + (self->length * sizeof(self->value[0]));
}

void
prefixed_string_free(struct prefixed_string *self) {
  if (!self || !self->value) {
    return;
  }

  free(self->value);
  self->value  = NULL;
  self->length = 0;
}

void
prefixed_wstring_free(struct prefixed_wstring *self) {
  if (!self || !self->value) {
    return;
  }

  free(self->value);
  self->value  = NULL;
  self->length = 0;
}

size_t
prefixed_string_compare(const struct prefixed_string* a,
                        const struct prefixed_string* b) {
  if (a == b) {
    return 1;
  }

  if (!a || !b) {
    return 0;
  }

  if (a->length != b->length) {
    return 0;
  }

  if (a->length == 0) {
    return 1;
  }

  return (memcmp(a->value, b->value, a->length * sizeof(a->value[0])) == 0);
}

size_t
prefixed_wstring_compare(const struct prefixed_wstring* a,
                         const struct prefixed_wstring* b) {
  if (a == b) {
    return 1;
  }

  if (!a || !b) {
    return 0;
  }

  if (a->length != b->length) {
    return 0;
  }

  if (a->length == 0) {
    return 1;
  }

  return (memcmp(a->value, b->value, a->length * sizeof(a->value[0])) == 0);
}

size_t
prefixed_string_hash(const struct prefixed_string* str) {
  if (!str || !str->value) {
    return 0;
  }

  size_t hash = str->length;
  const uint8_t *data = (const uint8_t *) str->value;
  const uint16_t len = str->length;

  for (uint16_t i = 0; i < len; i++) {
    hash += data[i];
  }

  return hash;
}

size_t
prefixed_wstring_hash(const struct prefixed_wstring* str) {
  if (!str || !str->value) {
    return 0;
  }

  size_t hash = str->length;
  const uint8_t *data = (const uint8_t *) str->value;
  const uint16_t len = str->length * sizeof(uint16_t);

  for (uint16_t i = 0; i < len; i++) {
    hash += data[i];
  }

  return hash;
}

size_t
prefixed_string_print(const struct prefixed_string const *self) {
  if (!self || !self->value) {
    printf("(null)");
    return 0;
  }

  printf("%s", self->value);
  return (size_t) self->length;
}

size_t
prefixed_wstring_print(const struct prefixed_wstring const *self) {
  if (!self || !self->value) {
    printf("(null)");
    return 0;
  }

  for (uint16_t i = 0; i < self->length; ++i) {
    printf("%lc", (wint_t) self->value[i]);
  }

  return (size_t) self->length;
}
