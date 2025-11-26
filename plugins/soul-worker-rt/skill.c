#include "skill.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct skill*
skill_new(void) {
  struct skill* self = malloc(sizeof(struct skill));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
skill_read(FILE* file, struct skill* self) {
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

  size_t info_read = prefixed_wstring_read(file, &self->info);
  if (info_read == 0) {
    prefixed_wstring_free(&self->name);
    return 0;
  }

  if (!safe_size_add(total_size, info_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    return 0;
  }

  size_t buff_read = prefixed_wstring_read(file, &self->buff);
  if (buff_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    return 0;
  }

  if (!safe_size_add(total_size, buff_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    return 0;
  }

  size_t icon_read = prefixed_wstring_read(file, &self->icon);
  if (icon_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    return 0;
  }

  if (!safe_size_add(total_size, icon_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
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

  if (!safe_size_add(total_size, sizeof(self->unknown1), &total_size)) {
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

  if (!safe_size_add(total_size, sizeof(self->unknown2), &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  size_t unknown3_read = prefixed_wstring_read(file, &self->description_image);
  if (unknown3_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  if (!safe_size_add(total_size, unknown3_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->description_image);
    return 0;
  }

  size_t movie_read = prefixed_wstring_read(file, &self->movie);
  if (movie_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->description_image);
    return 0;
  }

  if (!safe_size_add(total_size, movie_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->info);
    prefixed_wstring_free(&self->buff);
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->description_image);
    prefixed_wstring_free(&self->movie);
    return 0;
  }

  return total_size;
}

size_t
skill_write(FILE* file, struct skill const* self) {
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

  size_t info_write = prefixed_wstring_write(file, &self->info);
  VALIDATE_READ(info_write > 0, (void)0);

  if (!safe_size_add(total_size, info_write, &total_size)) {
    return 0;
  }

  size_t buff_write = prefixed_wstring_write(file, &self->buff);
  VALIDATE_READ(buff_write > 0, (void)0);

  if (!safe_size_add(total_size, buff_write, &total_size)) {
    return 0;
  }

  size_t icon_write = prefixed_wstring_write(file, &self->icon);
  VALIDATE_READ(icon_write > 0, (void)0);

  if (!safe_size_add(total_size, icon_write, &total_size)) {
    return 0;
  }

  size_t u1_write = fwrite(&self->unknown1, sizeof(self->unknown1), 1, file);
  VALIDATE_READ(u1_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown1), &total_size)) {
    return 0;
  }

  size_t u2_write = fwrite(&self->unknown2, sizeof(self->unknown2), 1, file);
  VALIDATE_READ(u2_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown2), &total_size)) {
    return 0;
  }

  size_t u3_write = prefixed_wstring_write(file, &self->description_image);
  VALIDATE_READ(u3_write > 0, (void)0);

  if (!safe_size_add(total_size, u3_write, &total_size)) {
    return 0;
  }

  size_t movie_write = prefixed_wstring_write(file, &self->movie);
  VALIDATE_READ(movie_write > 0, (void)0);

  if (!safe_size_add(total_size, movie_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
skill_free(struct skill* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->info);
  prefixed_wstring_free(&self->buff);
  prefixed_wstring_free(&self->icon);
  prefixed_wstring_free(&self->description_image);
  prefixed_wstring_free(&self->movie);
  free(self);
}

size_t
skill_hash(struct skill const* self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->name);
  hash += prefixed_wstring_hash(&self->info);
  hash += prefixed_wstring_hash(&self->buff);
  hash += prefixed_wstring_hash(&self->icon);
  hash += self->unknown1;
  hash += self->unknown2;
  hash += prefixed_wstring_hash(&self->description_image);
  hash += prefixed_wstring_hash(&self->movie);
  return hash;
}

void
skill_print(struct skill const* self) {
  if (!self) {
    printf("Skill: (null)\n");
    return;
  }

  printf("Skill ID: %u\n", self->id);
  printf("Name: %ls\n", self->name.length > 1 ? self->name.value : L"(None)");
  printf("Info: %ls\n", self->info.length > 1 ? self->info.value : L"(None)");
  printf("Buff: %ls\n", self->buff.length > 1 ? self->buff.value : L"(None)");
  printf("Icon: %ls\n", self->icon.length > 1 ? self->icon.value : L"(None)");
  printf("Unknown1: %u\n", self->unknown1);
  printf("Unknown2: %u\n", self->unknown2);
  printf("Description Image: %ls\n",
         self->description_image.length > 1 ? self->description_image.value : L"(None)");
  printf("Movie: %ls\n", self->movie.length > 1 ? self->movie.value : L"(None)");
}

size_t
skill_size(struct skill const* self) {
  if (!self)
    return 0;
  return sizeof(self->id) +
         prefixed_wstring_size(&self->name) +
         prefixed_wstring_size(&self->info) +
         prefixed_wstring_size(&self->buff) +
         prefixed_wstring_size(&self->icon) +
         sizeof(self->unknown1) +
         sizeof(self->unknown2) +
         prefixed_wstring_size(&self->description_image) +
         prefixed_wstring_size(&self->movie);
}
