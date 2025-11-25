#include "shop.h"
#include <stdlib.h>
#include <string.h>

struct shop *
shop_new(void) {
  struct shop *self = malloc(sizeof(struct shop));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
shop_read_from_file(FILE *file, struct shop *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) return 0;

  return sizeof(self->id) + name_read;
}

size_t
shop_write_to_file(FILE *file, struct shop const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t name_write = prefixed_wstring_write_to_file(file, &self->name);
  if (name_write == 0) return 0;
  write_count += name_write;

  return write_count;
}

size_t
shop_free(struct shop *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->name);
  free(self);
  return 0;
}

size_t
shop_hash(struct shop const *self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->name);
  return hash;
}
