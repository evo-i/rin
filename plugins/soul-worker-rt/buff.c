#include "buff.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

struct buff *
buff_new(void) {
  struct buff *self
    = malloc(sizeof(struct buff));

  if (!self) {
    return NULL;
  }

  (void) memset(self, 0, sizeof(*self));

  return self;
}

size_t
buff_read_from_file(FILE *file, struct buff *self) {
  size_t read_count = 0;

  read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) {
    printf("Failed to read buff ID\n");
    return 0;
  }

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) {
    printf("Failed to read buff name for ID %" PRIu32 "\n", self->id);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read_from_file(file, &self->description);
  if (desc_read == 0) {
    printf("Failed to read buff desc for ID %" PRIu32 "\n", self->id);
    prefixed_wstring_free(&self->name);
    return 0;
  }

  size_t icon_read = prefixed_wstring_read_from_file(file, &self->icon);
  if (icon_read == 0) {
    printf("Failed to read buff icon for ID %" PRIu32 "\n", self->id);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  size_t tip_read = prefixed_wstring_read_from_file(file, &self->tip);
  if (tip_read == 0) {
    printf("Failed to read buff tip for ID %" PRIu32 "\n", self->id);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  return
    sizeof(self->id)
      + name_read
      + desc_read
      + icon_read
      + tip_read;
}

size_t
buff_write_to_file(FILE *file, struct buff const *self) {
  size_t write_count = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) {
    return 0;
  }

  write_count += id_write;

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

  size_t icon_write = prefixed_wstring_write_to_file(file, &self->icon);
  if (icon_write == 0) {
    return 0;
  }

  write_count += icon_write;

  size_t tip_write = prefixed_wstring_write_to_file(file, &self->tip);
  if (tip_write == 0) {
    return 0;
  }

  write_count += tip_write;

  return write_count;
}

size_t
buff_free(struct buff *self) {
  if (!self) {
    return 0;
  }

  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->icon);
  prefixed_wstring_free(&self->tip);

  free(self);
  return 0;
}

size_t
buff_print(struct buff const *self) {
  if (!self) {
    return 0;
  }

  printf("Buff ID: %" PRIu32 "\n", self->id);
  printf("Name: ");
  prefixed_wstring_print(&self->name);
  printf("\nDescription: ");
  prefixed_wstring_print(&self->description);
  printf("\nIcon: ");
  prefixed_wstring_print(&self->icon);
  printf("\nTip: ");
  prefixed_wstring_print(&self->tip);
  printf("\n");

  return 0;
}

size_t
buff_compare(struct buff const *a,
             struct buff const *b) {
  if (a == b) {
    return 1;
  }

  if (!a || !b) {
    return 0;
  }

  if (a->id != b->id) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->name, &b->name)) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->description, &b->description)) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->icon, &b->icon)) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->tip, &b->tip)) {
    return 0;
  }

  return 1;
}

size_t
buff_hash(struct buff const *self) {
  if (!self) {
    return 0;
  }

  size_t h = 0;

  h += prefixed_wstring_hash(&self->name);
  h += prefixed_wstring_hash(&self->description);
  h += prefixed_wstring_hash(&self->icon);
  h += prefixed_wstring_hash(&self->tip);
  h += self->id;

  return h;
}

size_t
buff_size(struct buff const *self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
      + prefixed_wstring_size(&self->name)
      + prefixed_wstring_size(&self->description)
      + prefixed_wstring_size(&self->icon)
      + prefixed_wstring_size(&self->tip);
}

size_t
buff_copy(struct buff *dest,
          struct buff const *src) {
  if (!dest || !src) {
    return 0;
  }

  dest->id = src->id;
  dest->name.length = 0;
  dest->name.value = NULL;
  dest->description.length = 0;
  dest->description.value = NULL;
  dest->icon.length = 0;
  dest->icon.value = NULL;
  dest->tip.length = 0;
  dest->tip.value = NULL;

  if (prefixed_wstring_copy(&dest->name, &src->name) == 0) {
    return 0;
  }

  if (prefixed_wstring_copy(&dest->description, &src->description) == 0) {
    prefixed_wstring_free(&dest->name);
    return 0;
  }

  if (prefixed_wstring_copy(&dest->icon, &src->icon) == 0) {
    prefixed_wstring_free(&dest->name);
    prefixed_wstring_free(&dest->description);
    return 0;
  }

  if (prefixed_wstring_copy(&dest->tip, &src->tip) == 0) {
    prefixed_wstring_free(&dest->name);
    prefixed_wstring_free(&dest->description);
    prefixed_wstring_free(&dest->icon);
    return 0;
  }

  return buff_size(dest);
}
