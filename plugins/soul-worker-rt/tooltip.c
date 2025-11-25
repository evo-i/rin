#include "tooltip.h"
#include <stdlib.h>
#include <string.h>

struct tooltip *
tooltip_new(void) {
  struct tooltip *self = malloc(sizeof(struct tooltip));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
tooltip_read_from_file(FILE *file, struct tooltip *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) return 0;

  size_t desc_read = prefixed_wstring_read_from_file(file, &self->description);
  if (desc_read == 0) {
    prefixed_wstring_free(&self->name);
    return 0;
  }

  return sizeof(self->id) + name_read + desc_read;
}

size_t
tooltip_write_to_file(FILE *file, struct tooltip const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t name_write = prefixed_wstring_write_to_file(file, &self->name);
  if (name_write == 0) return 0;
  write_count += name_write;

  size_t desc_write = prefixed_wstring_write_to_file(file, &self->description);
  if (desc_write == 0) return 0;
  write_count += desc_write;

  return write_count;
}

size_t
tooltip_free(struct tooltip *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  free(self);
  return 0;
}

size_t
tooltip_hash(struct tooltip const *self) {
  if (!self) return 0;
  return self->id + prefixed_wstring_hash(&self->name) + prefixed_wstring_hash(&self->description);
}
