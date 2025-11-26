#include "booster.h"
#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

struct booster *
booster_new(void) {
  struct booster *self
    = malloc(sizeof(struct booster));

  if (!self) {
    return NULL;
  }

  (void) memset(self, 0, sizeof(*self));

  return self;
}

size_t
booster_read(FILE *file, struct booster *self) {
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

  return total_size;
}

size_t
booster_write(FILE *file, struct booster const *self) {
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

  return total_size;
}

void
booster_free(struct booster *self) {
  if (!self) {
    return;
  }

  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->icon);

  free(self);
}

void
booster_print(struct booster const *self) {
  if (!self) {
    return;
  }

  printf("Booster ID: %" PRIu16 "\n", self->id);
  printf("Name: ");
  prefixed_wstring_print(&self->name);
  printf("\nDesc: ");
  prefixed_wstring_print(&self->description);
  printf("\nIcon: ");
  prefixed_wstring_print(&self->icon);
  printf("\n");
}

size_t
booster_hash(struct booster const *self) {
  if (!self) {
    return 0;
  }

  size_t h = 0;

  h += prefixed_wstring_hash(&self->name);
  h += prefixed_wstring_hash(&self->description);
  h += prefixed_wstring_hash(&self->icon);
  h += self->id;

  return h;
}

size_t
booster_size(struct booster const *self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
      + prefixed_wstring_size(&self->name)
      + prefixed_wstring_size(&self->description)
      + prefixed_wstring_size(&self->icon);
}
