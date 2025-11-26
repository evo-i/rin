#include "monster.h"
#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct monster*
monster_new(void) {
  struct monster* self = malloc(sizeof(struct monster));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
monster_read(FILE* file, struct monster* self) {
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

  return total_size;
}

size_t
monster_write(FILE* file, struct monster const* self) {
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

  return total_size;
}

void
monster_free(struct monster* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->name);
  free(self);
}

size_t
monster_hash(struct monster const* self) {
  if (!self)
    return 0;
  return self->id + prefixed_wstring_hash(&self->name);
}

void
monster_print(struct monster const* self) {
  if (!self) {
    printf("Monster: (null)\n");
    return;
  }
  printf("Monster ID: %" PRIu32 "\n", self->id);
  prefixed_wstring_print(&self->name);
  printf("\n");
}

size_t
monster_size(struct monster const* self) {
  if (!self)
    return 0;
  return sizeof(self->id) + prefixed_wstring_size(&self->name);
}
