#include "achievement.h"
#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

struct achievement *
achievement_new(void) {
  struct achievement *self
    = malloc(sizeof(struct achievement));

  if (!self) {
    return NULL;
  }

  (void) memset(self, 0, sizeof(*self));

  return self;
}

size_t
achievement_read(FILE *file, struct achievement *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;
  size_t temp_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t title_read = prefixed_wstring_read(file, &self->title);
  VALIDATE_READ(title_read > 0, (void)0);

  if (!safe_size_add(total_size, title_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read(file, &self->description);
  VALIDATE_READ(desc_read > 0, prefixed_wstring_free(&self->title));

  if (!safe_size_add(total_size, desc_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  return total_size;
}

size_t
achievement_write(FILE *file, struct achievement const *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t title_write = prefixed_wstring_write(file, &self->title);
  VALIDATE_READ(title_write > 0, (void)0);

  if (!safe_size_add(total_size, title_write, &total_size)) {
    return 0;
  }

  size_t desc_write = prefixed_wstring_write(file, &self->description);
  VALIDATE_READ(desc_write > 0, (void)0);

  if (!safe_size_add(total_size, desc_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
achievement_free(struct achievement *self) {
  if (!self) {
    return;
  }

  prefixed_wstring_free(&self->title);
  prefixed_wstring_free(&self->description);

  free(self);
}

void
achievement_print(struct achievement const *self) {
  if (!self) {
    printf("(null achievement)\n");
    return;
  }

  printf("Achievement ID: %" PRIu32 "\n", self->id);
  printf("Title: ");
  prefixed_wstring_print(&self->title);
  printf("\nDescription: ");
  prefixed_wstring_print(&self->description);
  printf("\n");
}

size_t
achievement_hash(struct achievement const *self) {
  if (!self) {
    return 0;
  }

  size_t h = 0;

  h += prefixed_wstring_hash(&self->title);
  h += prefixed_wstring_hash(&self->description);
  h += self->id;

  return h;
}

size_t
achievement_size(struct achievement const *self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
      + prefixed_wstring_size(&self->title)
      + prefixed_wstring_size(&self->description);
}
