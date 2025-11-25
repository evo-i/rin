#include "speech.h"
#include <stdlib.h>
#include <string.h>

struct speech *
speech_new(void) {
  struct speech *self = malloc(sizeof(struct speech));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
speech_read_from_file(FILE *file, struct speech *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t text_read = prefixed_wstring_read_from_file(file, &self->text);
  if (text_read == 0) return 0;

  return sizeof(self->id) + text_read;
}

size_t
speech_write_to_file(FILE *file, struct speech const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t text_write = prefixed_wstring_write_to_file(file, &self->text);
  if (text_write == 0) return 0;
  write_count += text_write;

  return write_count;
}

size_t
speech_free(struct speech *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->text);
  free(self);
  return 0;
}

size_t
speech_hash(struct speech const *self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->text);
  return hash;
}
