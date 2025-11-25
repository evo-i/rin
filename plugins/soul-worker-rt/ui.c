#include "ui.h"
#include <stdlib.h>
#include <string.h>

struct ui *
ui_new(void) {
  struct ui *self = malloc(sizeof(struct ui));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
ui_read_from_file(FILE *file, struct ui *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  read_count = fread(&self->has_format, sizeof(self->has_format), 1, file);
  if (read_count != 1) return 0;

  size_t text_read = prefixed_wstring_read_from_file(file, &self->text);
  if (text_read == 0) return 0;

  return sizeof(self->id) + sizeof(self->has_format) + text_read;
}

size_t
ui_write_to_file(FILE *file, struct ui const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t format_write = fwrite(&self->has_format, sizeof(self->has_format), 1, file);
  if (format_write != 1) return 0;
  write_count += format_write * sizeof(self->has_format);

  size_t text_write = prefixed_wstring_write_to_file(file, &self->text);
  if (text_write == 0) return 0;
  write_count += text_write;

  return write_count;
}

size_t
ui_free(struct ui *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->text);
  free(self);
  return 0;
}

size_t
ui_hash(struct ui const *self) {
  if (!self) return 0;
  return self->id + self->has_format + prefixed_wstring_hash(&self->text);
}
