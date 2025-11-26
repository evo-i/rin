#include "title.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct title*
title_new(void) {
  struct title* self = malloc(sizeof(struct title));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
title_read(FILE* file, struct title* self) {
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
  if (desc_read == 0) {
    prefixed_wstring_free(&self->name);
    return 0;
  }

  if (!safe_size_add(total_size, desc_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  size_t tip_read = prefixed_wstring_read(file, &self->tip);
  if (tip_read == 0) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    return 0;
  }

  if (!safe_size_add(total_size, tip_read, &total_size)) {
    prefixed_wstring_free(&self->name);
    prefixed_wstring_free(&self->description);
    prefixed_wstring_free(&self->tip);
    return 0;
  }

  return total_size;
}

size_t
title_write(FILE* file, struct title const* self) {
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

  size_t tip_write = prefixed_wstring_write(file, &self->tip);
  VALIDATE_READ(tip_write > 0, (void)0);

  if (!safe_size_add(total_size, tip_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
title_free(struct title* self) {
  if (!self) {
    return;
  }

  prefixed_wstring_free(&self->name);
  prefixed_wstring_free(&self->description);
  prefixed_wstring_free(&self->tip);
  free(self);
}

size_t
title_hash(struct title const* self) {
  if (!self) {
    return 0;
  }

  return
    self->id
      + prefixed_wstring_hash(&self->name)
      + prefixed_wstring_hash(&self->description)
      + prefixed_wstring_hash(&self->tip);
}

size_t
title_size(struct title const* self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
      + prefixed_wstring_size(&self->name)
      + prefixed_wstring_size(&self->description)
      + prefixed_wstring_size(&self->tip);
}
