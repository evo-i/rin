#include "speech.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct speech*
speech_new(void) {
  struct speech* self = malloc(sizeof(struct speech));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
speech_read(FILE* file, struct speech* self) {
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
speech_write(FILE* file, struct speech const* self) {
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
speech_free(struct speech* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->text);
  free(self);
}

size_t
speech_hash(struct speech const* self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->text);
  return hash;
}

size_t
speech_size(struct speech const* self) {
  if (!self)
    return 0;
  return sizeof(self->id) + prefixed_wstring_size(&self->text);
}
