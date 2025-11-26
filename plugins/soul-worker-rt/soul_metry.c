#include "soul_metry.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct soul_metry*
soul_metry_new(void) {
  struct soul_metry* self = malloc(sizeof(struct soul_metry));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
soul_metry_read(FILE* file, struct soul_metry* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t text_read = prefixed_wstring_read(file, &self->text);
  VALIDATE_READ(text_read > 0, (void)0);

  if (!safe_size_add(total_size, text_read, &total_size)) {
    prefixed_wstring_free(&self->text);
    return 0;
  }

  return total_size;
}

size_t
soul_metry_write(FILE* file, struct soul_metry const* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t text_write = prefixed_wstring_write(file, &self->text);
  VALIDATE_READ(text_write > 0, (void)0);

  if (!safe_size_add(total_size, text_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
soul_metry_free(struct soul_metry* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->text);
  free(self);
}

size_t
soul_metry_hash(struct soul_metry const* self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->text);
  return hash;
}

size_t
soul_metry_size(struct soul_metry const* self) {
  if (!self)
    return 0;
  return sizeof(self->id) + prefixed_wstring_size(&self->text);
}
