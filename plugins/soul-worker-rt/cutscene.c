#include "cutscene.h"
#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

struct cutscene *
cutscene_new(void) {
  struct cutscene *self
    = malloc(sizeof(struct cutscene));

  if (!self) {
    return NULL;
  }

  (void) memset(self, 0, sizeof(*self));

  return self;
}

size_t
cutscene_read(FILE *file, struct cutscene *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t text_read = prefixed_wstring_read(file, &self->text);
  VALIDATE_READ(text_read > 0, (void)0);

  if (!safe_size_add(total_size, text_read, &total_size)) {
    prefixed_wstring_free(&self->text);
    return 0;
  }

  return total_size;
}

size_t
cutscene_write(FILE *file, struct cutscene const *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t text_write = prefixed_wstring_write(file, &self->text);
  VALIDATE_READ(text_write > 0, (void)0);

  if (!safe_size_add(total_size, text_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
cutscene_free(struct cutscene *self) {
  if (!self) {
    return;
  }

  prefixed_wstring_free(&self->text);

  free(self);
}

void
cutscene_print(struct cutscene const *self) {
  if (!self) {
    return;
  }

  printf("Cutscene ID: %" PRIu32 "\n", self->id);
  prefixed_wstring_print(&self->text);
  printf("\n");
}

size_t
cutscene_hash(struct cutscene const *self) {
  if (!self) {
    return 0;
  }

  size_t h = 0;

  h += prefixed_wstring_hash(&self->text);
  h += self->id;

  return h;
}

size_t
cutscene_size(struct cutscene const *self) {
  if (!self) {
    return 0;
  }

  return
    sizeof(self->id)
      + prefixed_wstring_size(&self->text);
}
