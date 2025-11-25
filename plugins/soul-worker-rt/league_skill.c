#include "league_skill.h"
#include <stdlib.h>
#include <string.h>

struct league_skill *
league_skill_new(void) {
  struct league_skill *self = malloc(sizeof(struct league_skill));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
league_skill_read_from_file(FILE *file, struct league_skill *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t icon_read = prefixed_wstring_read_from_file(file, &self->icon);
  if (icon_read == 0) return 0;

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) {
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  size_t unknown_read = prefixed_wstring_read_from_file(file, &self->unknown);
  if (unknown_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read_from_file(file, &self->description);
  if (desc_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->unknown);
    return 0;
  }

  size_t title_read = prefixed_wstring_read_from_file(file, &self->title);
  if (title_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->unknown);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  return sizeof(self->id) + icon_read + name_read + unknown_read + desc_read + title_read;
}

size_t
league_skill_write_to_file(FILE *file, struct league_skill const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t icon_write = prefixed_wstring_write_to_file(file, &self->icon);
  if (icon_write == 0) return 0;
  write_count += icon_write;

  size_t name_write = prefixed_wstring_write_to_file(file, &self->name);
  if (name_write == 0) return 0;
  write_count += name_write;

  size_t unknown_write = prefixed_wstring_write_to_file(file, &self->unknown);
  if (unknown_write == 0) return 0;
  write_count += unknown_write;

  size_t desc_write = prefixed_wstring_write_to_file(file, &self->description);
  if (desc_write == 0) return 0;
  write_count += desc_write;

  size_t title_write = prefixed_wstring_write_to_file(file, &self->title);
  if (title_write == 0) return 0;
  write_count += title_write;

  return write_count;
}

size_t
league_skill_free(struct league_skill *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->icon);
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->unknown);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->title);
  free(self);
  return 0;
}

size_t
league_skill_hash(struct league_skill const *self) {
  if (!self) return 0;
  return self->id
       + prefixed_wstring_hash(&self->icon)
       + prefixed_wstring_hash(&self->name)
       + prefixed_wstring_hash(&self->unknown)
       + prefixed_wstring_hash(&self->description)
       + prefixed_wstring_hash(&self->title);
}
