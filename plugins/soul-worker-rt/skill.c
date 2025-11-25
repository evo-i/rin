#include "skill.h"
#include <stdlib.h>
#include <string.h>

struct skill *
skill_new(void) {
  struct skill *self = malloc(sizeof(struct skill));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
skill_read_from_file(FILE *file, struct skill *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) return 0;

  size_t info_read = prefixed_wstring_read_from_file(file, &self->info);
  if (info_read == 0) {
    prefixed_wstring_free(&self->name);
    return 0;
  }

  size_t buff_read = prefixed_wstring_read_from_file(file, &self->buff);
  if (buff_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    return 0;
  }

  size_t icon_read = prefixed_wstring_read_from_file(file, &self->icon);
  if (icon_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    return 0;
  }

  read_count = fread(&self->unknown1, sizeof(self->unknown1), 1, file);
  if (read_count != 1) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  read_count = fread(&self->unknown2, sizeof(self->unknown2), 1, file);
  if (read_count != 1) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  size_t unknown3_read = prefixed_wstring_read_from_file(file, &self->unknown3);
  if (unknown3_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  size_t movie_read = prefixed_wstring_read_from_file(file, &self->movie);
  if (movie_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->unknown3);
    return 0;
  }

  return sizeof(self->id) + name_read + info_read + buff_read + icon_read + sizeof(self->unknown1) + sizeof(self->unknown2) + unknown3_read + movie_read;
}

size_t
skill_write_to_file(FILE *file, struct skill const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t name_write = prefixed_wstring_write_to_file(file, &self->name);
  if (name_write == 0) return 0;
  write_count += name_write;

  size_t info_write = prefixed_wstring_write_to_file(file, &self->info);
  if (info_write == 0) return 0;
  write_count += info_write;

  size_t buff_write = prefixed_wstring_write_to_file(file, &self->buff);
  if (buff_write == 0) return 0;
  write_count += buff_write;

  size_t icon_write = prefixed_wstring_write_to_file(file, &self->icon);
  if (icon_write == 0) return 0;
  write_count += icon_write;

  size_t u1_write = fwrite(&self->unknown1, sizeof(self->unknown1), 1, file);
  if (u1_write != 1) return 0;
  write_count += u1_write * sizeof(self->unknown1);

  size_t u2_write = fwrite(&self->unknown2, sizeof(self->unknown2), 1, file);
  if (u2_write != 1) return 0;
  write_count += u2_write * sizeof(self->unknown2);

  size_t u3_write = prefixed_wstring_write_to_file(file, &self->unknown3);
  if (u3_write == 0) return 0;
  write_count += u3_write;

  size_t movie_write = prefixed_wstring_write_to_file(file, &self->movie);
  if (movie_write == 0) return 0;
  write_count += movie_write;

  return write_count;
}

size_t
skill_free(struct skill *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->info);
  prefixed_wstring_free(&self->buff);
  prefixed_wstring_free(&self->icon);
  prefixed_wstring_free(&self->unknown3);
  prefixed_wstring_free(&self->movie);
  free(self);
  return 0;
}

size_t
skill_hash(struct skill const *self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->name);
  hash += prefixed_wstring_hash(&self->info);
  hash += prefixed_wstring_hash(&self->buff);
  hash += prefixed_wstring_hash(&self->icon);
  hash += self->unknown1;
  hash += self->unknown2;
  hash += prefixed_wstring_hash(&self->unknown3);
  hash += prefixed_wstring_hash(&self->movie);
  return hash;
}
