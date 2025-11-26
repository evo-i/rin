#include "buff.h"
#include "common.h"

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
buff_read(FILE *file, struct buff *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t name_read = prefixed_wstring_read(file, &self->name);
  VALIDATE_READ(name_read > 0, (void)0);

  if (!safe_size_add(total_size, name_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read(file, &self->description);
  VALIDATE_READ(desc_read > 0, prefixed_wstring_free(&self->name));

  if (!safe_size_add(total_size, desc_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  size_t icon_read = prefixed_wstring_read(file, &self->icon);
  VALIDATE_READ(icon_read > 0, {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
  });

  if (!safe_size_add(total_size, icon_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  size_t tip_read = prefixed_wstring_read(file, &self->tip);
  VALIDATE_READ(tip_read > 0, {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    prefixed_wstring_free(&self->icon);
  });

  if (!safe_size_add(total_size, tip_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->tip);
    return 0;
  }

  return total_size;
}

size_t
buff_write(FILE *file, struct buff const *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t name_write = prefixed_wstring_write(file, &self->name);
  VALIDATE_READ(name_write > 0, (void)0);

  if (!safe_size_add(total_size, name_write, &total_size)) {
    return 0;
  }

  size_t desc_write = prefixed_wstring_write(file, &self->description);
  VALIDATE_READ(desc_write > 0, (void)0);

  if (!safe_size_add(total_size, desc_write, &total_size)) {
    return 0;
  }

  size_t icon_write = prefixed_wstring_write(file, &self->icon);
  VALIDATE_READ(icon_write > 0, (void)0);

  if (!safe_size_add(total_size, icon_write, &total_size)) {
    return 0;
  }

  size_t tip_write = prefixed_wstring_write(file, &self->tip);
  VALIDATE_READ(tip_write > 0, (void)0);

  if (!safe_size_add(total_size, tip_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
buff_free(struct buff *self) {
  if (!self) {
    return;
  }

  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->icon);
  prefixed_wstring_free(&self->tip);

  free(self);
}

void
buff_print(struct buff const *self) {
  if (!self) {
    return;
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
