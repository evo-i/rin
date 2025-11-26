#include "ui.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct ui*
ui_new(void) {
  struct ui* self = malloc(sizeof(struct ui));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
ui_read(FILE* file, struct ui* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  read_count = fread(&self->unknown1, sizeof(self->unknown1), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown1), &total_size)) {
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
ui_write(FILE* file, struct ui const* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t format_write = fwrite(&self->unknown1, sizeof(self->unknown1), 1, file);
  VALIDATE_READ(format_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown1), &total_size)) {
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
ui_free(struct ui* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->text);
  free(self);
}

size_t
ui_hash(struct ui const* self) {
  if (!self)
    return 0;
  return
    self->id
      + self->unknown1
      + prefixed_wstring_hash(&self->text);
}

void
ui_print(struct ui const* self) {
  if (!self) {
    return;
  }

  printf("UI ID: %" PRIu32 "\n", self->id);
  printf("Unknown1: %" PRIu8 "\n", self->unknown1);
  printf("Text: ");
  prefixed_wstring_print(&self->text);
  printf("\n");
}

size_t
ui_size(struct ui const* self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
      + sizeof(self->unknown1)
      + prefixed_wstring_size(&self->text);
}
