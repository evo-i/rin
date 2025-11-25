#include "title.h"
#include <stdlib.h>
#include <string.h>

struct title *
title_new(void) {
  struct title *self = malloc(sizeof(struct title));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
title_read_from_file(FILE *file, struct title *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);

  if (read_count != 1) {
    return 0;
  }

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) {
    return 0;
  }

  size_t desc_read = prefixed_wstring_read_from_file(file, &self->description);

  if (desc_read == 0) {
    prefixed_wstring_free(&self->name);
    return 0;
  }

  size_t tip_read
    = prefixed_wstring_read_from_file(file, &self->tip);

  if (tip_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  return sizeof(self->id) + name_read + desc_read + tip_read;
}

size_t
title_write_to_file(FILE *file, struct title const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);

  if (id_write != 1) {
    return 0;
  }

  write_count += id_write * sizeof(self->id);

  size_t name_write = prefixed_wstring_write_to_file(file, &self->name);

  if (name_write == 0) {
    return 0;
  }

  write_count += name_write;

  size_t desc_write = prefixed_wstring_write_to_file(file, &self->description);

  if (desc_write == 0) {
    return 0;
  }

  write_count += desc_write;

  size_t tip_write = prefixed_wstring_write_to_file(file, &self->tip);

  if (tip_write == 0) {
    return 0;
  }

  write_count += tip_write;

  return write_count;
}

size_t
title_free(struct title *self) {
  if (!self) {
    return 0;
  }

  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->tip);
  free(self);
  return 0;
}

size_t
title_hash(struct title const *self) {
  if (!self) {
    return 0;
  }

  return
    self->id
      + prefixed_wstring_hash(&self->name)
      + prefixed_wstring_hash(&self->description)
      + prefixed_wstring_hash(&self->tip);
}
