#include "cutscene.h"

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
cutscene_read_from_file(FILE *file, struct cutscene *self) {
  size_t read_count = 0;

  read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) {
    return 0;
  }

  size_t text_read = prefixed_wstring_read_from_file(file, &self->text);
  if (text_read == 0) {
    return 0;
  }

  return
    sizeof(self->id)
      + text_read;
}

size_t
cutscene_write_to_file(FILE *file, struct cutscene const *self) {
  size_t write_count = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) {
    return 0;
  }

  write_count += id_write;

  size_t text_write = prefixed_wstring_write_to_file(file, &self->text);
  if (text_write == 0) {
    return 0;
  }

  write_count += text_write;

  return write_count;
}

size_t
cutscene_free(struct cutscene *self) {
  if (!self) {
    return 0;
  }

  prefixed_wstring_free(&self->text);

  free(self);
  return 0;
}

size_t
cutscene_print(struct cutscene const *self) {
  if (!self) {
    return 0;
  }

  printf("Cutscene ID: %" PRIu32 "\n", self->id);
  prefixed_wstring_print(&self->text);
  printf("\n");

  return 0;
}

size_t
cutscene_compare(struct cutscene const *a,
                 struct cutscene const *b) {
  if (a == b) {
    return 1;
  }

  if (!a || !b) {
    return 0;
  }

  if (a->id != b->id) {
    return 0;
  }

  if (!prefixed_wstring_compare(&a->text, &b->text)) {
    return 0;
  }

  return 1;
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

size_t
cutscene_copy(struct cutscene *dest,
              struct cutscene const *src) {
  if (!dest || !src) {
    return 0;
  }

  dest->id = src->id;
  dest->text.length = 0;
  dest->text.value = NULL;

  if (prefixed_wstring_copy(&dest->text, &src->text) == 0) {
    return 0;
  }

  return cutscene_size(dest);
}
