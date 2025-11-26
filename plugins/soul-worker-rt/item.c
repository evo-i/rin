#include "item.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct item*
item_new(void) {
  struct item* self = malloc(sizeof(struct item));
  if (!self) {
    return NULL;
  }

  memset(self, 0, sizeof(*self));
  return self;
}

static
size_t
item_resources_read_from_file(FILE* file, struct item_resources* res) {
  size_t total_read = 0;
  for (size_t i = 0; i < ITEM_RESOURCE_COUNT; i++) {
    size_t res_read = prefixed_wstring_read(file, &res->values[i]);

    if (res_read == 0) {
      return 0;
    }

    total_read += res_read;
  }

  size_t flags_read = fread(res->flags, sizeof(res->flags), 1, file);

  if (flags_read != 1) {
    return 0;
  }

  total_read += sizeof(res->flags);
  return total_read;
}

static
void
item_free_resources(struct item_resources* res) {
  for (size_t i = 0; i < ITEM_RESOURCE_COUNT; i++) {
    prefixed_wstring_free(&res->values[i]);
  }
}

static
size_t
item_resources_size(struct item_resources const* res) {
  size_t total_size = 0;

  for (size_t i = 0; i < ITEM_RESOURCE_COUNT; i++) {
    total_size += prefixed_wstring_size(&res->values[i]);
  }

  total_size += sizeof(res->flags);
  return total_size;
}

static
size_t
item_resources_hash(struct item_resources const* res) {
  size_t h = 0;

  for (size_t i = 0; i < ITEM_RESOURCE_COUNT; i++) {
    h += prefixed_wstring_hash(&res->values[i]);
    h += res->flags[i];
  }

  return h;
}

size_t
item_read(FILE* file, struct item* self) {
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

  size_t resources_read = item_resources_read_from_file(file, &self->resources);
  if (resources_read == 0) {
    prefixed_wstring_free(&self->icon);
    item_free_resources(&self->resources);
    return 0;
  }

  if (!safe_size_add(total_size, resources_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    item_free_resources(&self->resources);
    return 0;
  }

  size_t name_read = prefixed_wstring_read(file, &self->name);
  if (name_read == 0) {
    prefixed_wstring_free(&self->icon);
    item_free_resources(&self->resources);
    return 0;
  }

  if (!safe_size_add(total_size, name_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    item_free_resources(&self->resources);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read(file, &self->description);
  if (desc_read == 0) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    item_free_resources(&self->resources);
    return 0;
  }

  if (!safe_size_add(total_size, desc_read, &total_size)) {
    prefixed_wstring_free(&self->icon);
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    item_free_resources(&self->resources);
    return 0;
  }

  return total_size;
}

static
size_t
item_resources_write_to_file(FILE* file, struct item_resources const* res) {
  size_t total_written = 0;
  for (size_t i = 0; i < ITEM_RESOURCE_COUNT; i++) {
    total_written += prefixed_wstring_write(file, &res->values[i]);
  }

  total_written += fwrite(res->flags, sizeof(res->flags), 1, file);

  return total_written;
}

size_t
item_write(FILE* file, struct item const* self) {
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

  size_t resources_write = item_resources_write_to_file(file, &self->resources);
  VALIDATE_READ(resources_write > 0, (void)0);

  if (!safe_size_add(total_size, resources_write, &total_size)) {
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

  return total_size;
}

void
item_free(struct item* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->icon);
  item_free_resources(&self->resources);
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  free(self);
}

size_t
item_hash(struct item const* self) {
  if (!self)
    return 0;
  size_t h = self->id;
  h += prefixed_wstring_hash(&self->icon);
  h += item_resources_hash(&self->resources);

  h += prefixed_wstring_hash(&self->name);
  h += prefixed_wstring_hash(&self->description);
  return h;
}

size_t
item_size(struct item const* self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
    + prefixed_wstring_size(&self->icon)
    + item_resources_size(&self->resources)
    + prefixed_wstring_size(&self->name)
    + prefixed_wstring_size(&self->description);
}

size_t
item_print(struct item const* self) {
  if (!self) {
    return 0;
  }
  
  printf("ID: %u\n", self->id);
  printf("Icon: %ls\n", self->icon.length > 1 ? self->icon.value : L"(None)");

  for (size_t i = 0; i < ITEM_RESOURCE_COUNT; i++) {
    printf(" - Resource %zu: %ls (flag: %u)\n",
           i + 1,
           self->resources.values[i].length > 1
            ? self->resources.values[i].value
            : L"(None)",
           self->resources.flags[i]);
  }

  printf("Name: %ls\n", self->name.length > 1 ? self->name.value : L"(None)");
  printf("Description: \"%ls\"\n\n",
         self->description.length > 1 ? self->description.value : L"(None)");
}
