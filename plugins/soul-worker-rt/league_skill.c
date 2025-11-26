#include "league_skill.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct league_skill*
league_skill_new(void) {
  struct league_skill* self = malloc(sizeof(struct league_skill));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
league_skill_read(FILE* file, struct league_skill* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t icon_read = prefixed_wstring_read(file, &self->icon);
  VALIDATE_READ(icon_read > 0, (void)0);

  if (!safe_size_add(total_size, icon_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  size_t name_read = prefixed_wstring_read(file, &self->name);
  if (name_read == 0) {
    prefixed_wstring_free(&self->icon);
    return 0;
  }

  if (!safe_size_add(total_size, name_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    return 0;
  }

  size_t unknown_read = prefixed_wstring_read(file, &self->key);
  if (unknown_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    return 0;
  }

  if (!safe_size_add(total_size, unknown_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->key);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read(file, &self->description);
  if (desc_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->key);
    return 0;
  }

  if (!safe_size_add(total_size, desc_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->key);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  size_t title_read = prefixed_wstring_read(file, &self->title);
  if (title_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->key);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  if (!safe_size_add(total_size, title_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->key);
    prefixed_wstring_free(&self->description);
    prefixed_wstring_free(&self->title);
    return 0;
  }

  return total_size;
}

size_t
league_skill_write(FILE* file, struct league_skill const* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t icon_write = prefixed_wstring_write(file, &self->icon);
  VALIDATE_READ(icon_write > 0, (void)0);

  if (!safe_size_add(total_size, icon_write, &total_size)) {
    return 0;
  }

  size_t name_write = prefixed_wstring_write(file, &self->name);
  VALIDATE_READ(name_write > 0, (void)0);

  if (!safe_size_add(total_size, name_write, &total_size)) {
    return 0;
  }

  size_t unknown_write = prefixed_wstring_write(file, &self->key);
  VALIDATE_READ(unknown_write > 0, (void)0);

  if (!safe_size_add(total_size, unknown_write, &total_size)) {
    return 0;
  }

  size_t desc_write = prefixed_wstring_write(file, &self->description);
  VALIDATE_READ(desc_write > 0, (void)0);

  if (!safe_size_add(total_size, desc_write, &total_size)) {
    return 0;
  }

  size_t title_write = prefixed_wstring_write(file, &self->title);
  VALIDATE_READ(title_write > 0, (void)0);

  if (!safe_size_add(total_size, title_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
league_skill_free(struct league_skill* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->icon);
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->key);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->title);
  
  free(self);
}

size_t
league_skill_hash(struct league_skill const* self) {
  if (!self)
    return 0;
  return self->id + prefixed_wstring_hash(&self->icon) + prefixed_wstring_hash(&self->name) +
         prefixed_wstring_hash(&self->key) + prefixed_wstring_hash(&self->description) +
         prefixed_wstring_hash(&self->title);
}

void
league_skill_print(struct league_skill const* self) {
  if (!self) {
    printf("League Skill: (null)\n");
    return;
  }

  printf("League Skill ID: %u\n", self->id);
  printf("Icon: %ls\n", self->icon.length > 1 ? self->icon.value : L"(None)");
  printf("Name: %ls\n", self->name.length > 1 ? self->name.value : L"(None)");
  printf("Key: %ls\n", self->key.length > 1 ? self->key.value : L"(None)");
  printf("Description: %ls\n", self->description.length > 1 ? self->description.value : L"(None)");
  printf("Title: %ls\n", self->title.length > 1 ? self->title.value : L"(None)");
}

size_t
league_skill_size(struct league_skill const* self) {
  if (!self)
    return 0;

  size_t size = sizeof(self->id);
  size += prefixed_wstring_size(&self->icon);
  size += prefixed_wstring_size(&self->name);
  size += prefixed_wstring_size(&self->key);
  size += prefixed_wstring_size(&self->description);
  size += prefixed_wstring_size(&self->title);

  return size;
}
