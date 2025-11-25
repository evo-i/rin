#include "item.h"
#include <stdlib.h>
#include <string.h>

struct item *
item_new(void) {
  struct item *self = malloc(sizeof(struct item));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
item_read_from_file(FILE *file, struct item *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t u1_read = prefixed_wstring_read_from_file(file, &self->unknown1);
  if (u1_read == 0) return 0;

  size_t u2_read = prefixed_wstring_read_from_file(file, &self->unknown2);
  if (u2_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    return 0;
  }

  size_t u3_read = prefixed_wstring_read_from_file(file, &self->unknown3);
  if (u3_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    return 0;
  }

  size_t u4_read = prefixed_wstring_read_from_file(file, &self->unknown4);
  if (u4_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    prefixed_wstring_free(&self->unknown3);
    return 0;
  }

  size_t u5_read = prefixed_wstring_read_from_file(file, &self->unknown5);
  if (u5_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    prefixed_wstring_free(&self->unknown3);
    prefixed_wstring_free(&self->unknown4);
    return 0;
  }

  size_t u6_read = prefixed_wstring_read_from_file(file, &self->unknown6);
  if (u6_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    prefixed_wstring_free(&self->unknown3);
    prefixed_wstring_free(&self->unknown4);
    prefixed_wstring_free(&self->unknown5);
    return 0;
  }

  read_count = fread(self->flags, sizeof(self->flags), 1, file);
  if (read_count != 1) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    prefixed_wstring_free(&self->unknown3);
    prefixed_wstring_free(&self->unknown4);
    prefixed_wstring_free(&self->unknown5);
    prefixed_wstring_free(&self->unknown6);
    return 0;
  }

  size_t name_read = prefixed_wstring_read_from_file(file, &self->name);
  if (name_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    prefixed_wstring_free(&self->unknown3);
    prefixed_wstring_free(&self->unknown4);
    prefixed_wstring_free(&self->unknown5);
    prefixed_wstring_free(&self->unknown6);
    return 0;
  }

  size_t desc_read = prefixed_wstring_read_from_file(file, &self->description);
  if (desc_read == 0) {
    prefixed_wstring_free(&self->unknown1);
    prefixed_wstring_free(&self->unknown2);
    prefixed_wstring_free(&self->unknown3);
    prefixed_wstring_free(&self->unknown4);
    prefixed_wstring_free(&self->unknown5);
    prefixed_wstring_free(&self->unknown6);
    prefixed_wstring_free(&self->name);
    return 0;
  }

  return sizeof(self->id) + u1_read + u2_read + u3_read + u4_read + u5_read + u6_read + sizeof(self->flags) + name_read + desc_read;
}

size_t
item_write_to_file(FILE *file, struct item const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t u1_write = prefixed_wstring_write_to_file(file, &self->unknown1);
  if (u1_write == 0) return 0;
  write_count += u1_write;

  size_t u2_write = prefixed_wstring_write_to_file(file, &self->unknown2);
  if (u2_write == 0) return 0;
  write_count += u2_write;

  size_t u3_write = prefixed_wstring_write_to_file(file, &self->unknown3);
  if (u3_write == 0) return 0;
  write_count += u3_write;

  size_t u4_write = prefixed_wstring_write_to_file(file, &self->unknown4);
  if (u4_write == 0) return 0;
  write_count += u4_write;

  size_t u5_write = prefixed_wstring_write_to_file(file, &self->unknown5);
  if (u5_write == 0) return 0;
  write_count += u5_write;

  size_t u6_write = prefixed_wstring_write_to_file(file, &self->unknown6);
  if (u6_write == 0) return 0;
  write_count += u6_write;

  size_t flags_write = fwrite(self->flags, sizeof(self->flags), 1, file);
  if (flags_write != 1) return 0;
  write_count += flags_write * sizeof(self->flags);

  size_t name_write = prefixed_wstring_write_to_file(file, &self->name);
  if (name_write == 0) return 0;
  write_count += name_write;

  size_t desc_write = prefixed_wstring_write_to_file(file, &self->description);
  if (desc_write == 0) return 0;
  write_count += desc_write;

  return write_count;
}

size_t
item_free(struct item *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->unknown1);
  prefixed_wstring_free(&self->unknown2);
  prefixed_wstring_free(&self->unknown3);
  prefixed_wstring_free(&self->unknown4);
  prefixed_wstring_free(&self->unknown5);
  prefixed_wstring_free(&self->unknown6);
  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  free(self);
  return 0;
}

size_t
item_hash(struct item const *self) {
  if (!self) return 0;
  size_t h = self->id;
  h += prefixed_wstring_hash(&self->unknown1);
  h += prefixed_wstring_hash(&self->unknown2);
  h += prefixed_wstring_hash(&self->unknown3);
  h += prefixed_wstring_hash(&self->unknown4);
  h += prefixed_wstring_hash(&self->unknown5);
  h += prefixed_wstring_hash(&self->unknown6);
  for (int i = 0; i < 5; i++) {
    h += self->flags[i];
  }
  h += prefixed_wstring_hash(&self->name);
  h += prefixed_wstring_hash(&self->description);
  return h;
}

size_t
item_size(struct item const *self) {
  if (!self) return 0;
  return sizeof(self->id)
       + prefixed_wstring_size(&self->unknown1)
       + prefixed_wstring_size(&self->unknown2)
       + prefixed_wstring_size(&self->unknown3)
       + prefixed_wstring_size(&self->unknown4)
       + prefixed_wstring_size(&self->unknown5)
       + prefixed_wstring_size(&self->unknown6)
       + sizeof(self->flags)
       + prefixed_wstring_size(&self->name)
       + prefixed_wstring_size(&self->description);
}

size_t
item_print(struct item const *self) {
  if (!self) return 0;
  printf("Item ID: %u\n", self->id);
  return 0;
}

size_t
item_compare(struct item const *a, struct item const *b) {
  if (a == b) return 1;
  if (!a || !b) return 0;
  if (a->id != b->id) return 0;
  return 1;
}

size_t
item_copy(struct item *dest, struct item const *src) {
  if (!dest || !src) return 0;
  return 0;
}

