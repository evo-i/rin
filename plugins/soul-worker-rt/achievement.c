#include "achievement.h"

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
achievement_read_from_file(FILE *file, struct achievement *self) {
  size_t read_count = 0;

  read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) {
    return 0;
  }

  size_t title_read = prefixed_wstring_read_from_file(file, &self->title);
  if (title_read == 0) {
    return 0;
  }

  size_t desc_read = prefixed_wstring_read_from_file(file, &self->description);
  if (desc_read == 0) {
    free(self->title.value);
    memset(&self->title, 0, sizeof(self->title));
    return 0;
  }

  return
    sizeof(self->id)
      + title_read
      + desc_read;
}

size_t
achievement_write_to_file(FILE *file, struct achievement const *self) {
  size_t write_count = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) {
    return 0;
  }

  write_count += id_write;

  size_t title_write = prefixed_wstring_write_to_file(file, &self->title);
  if (title_write == 0) {
    return 0;
  }

  write_count += title_write;

  size_t desc_write = prefixed_wstring_write_to_file(file, &self->description);
  if (desc_write == 0) {
    return 0;
  }

  write_count += desc_write;

  return write_count;
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
achievement_compare(struct achievement const *a,
                    struct achievement const *b) {
  if (a == b) {
    return 1;
  }

  if (!a || !b) {
    return 0;
  }

  if (a->id != b->id) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->title, &b->title)) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->description, &b->description)) {
    return 0;
  }

  return 1;
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

size_t
achievement_copy(struct achievement *dest,
                 struct achievement const *src) {
  if (!dest || !src) {
    return 0;
  }

  dest->id = src->id;
  dest->title.length = 0;
  dest->title.value = NULL;
  dest->description.length = 0;
  dest->description.value = NULL;

  if (prefixed_wstring_copy(&dest->title, &src->title) == 0) {
    dest->title.length = 0;
    dest->title.value = NULL;
    return 0;
  }

  if (prefixed_wstring_copy(&dest->description, &src->description) == 0) {
    prefixed_wstring_free(&dest->title);
    dest->description.length = 0;
    dest->description.value = NULL;
    return 0;
  }

  return achievement_size(dest);
}
