#include "cinema.h"
#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

struct cinema *
cinema_new(void) {
  struct cinema *self
    = malloc(sizeof(struct cinema));

  if (!self) {
    return NULL;
  }

  (void) memset(self, 0, sizeof(*self));

  return self;
}

size_t
cinema_read(FILE *file, struct cinema *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  for (int i = 0; i < CINEMA_STRING_COUNT; ++i) {
    size_t n = prefixed_wstring_read(file, &self->strings[i]);
    if (n == 0) {
      for (int j = 0; j < i; ++j) {
        prefixed_wstring_free(&self->strings[j]);
      }
      return 0;
    }
    
    if (!safe_size_add(total_size, n, &total_size)) {
      for (int j = 0; j <= i; ++j) {
        prefixed_wstring_free(&self->strings[j]);
      }
      return 0;
    }
  }

  return total_size;
}

size_t
cinema_write(FILE *file, struct cinema const *self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  for (int i = 0; i < 12; ++i) {
    size_t w = prefixed_wstring_write(file, &self->strings[i]);
    VALIDATE_READ(w > 0, (void)0);
    
    if (!safe_size_add(total_size, w, &total_size)) {
      return 0;
    }
  }

  return total_size;
}

void
cinema_free(struct cinema *self) {
  if (!self) {
    return;
  }

  for (int i = 0; i < 12; ++i) {
    prefixed_wstring_free(&self->strings[i]);
  }

  free(self);
}

void
cinema_print(struct cinema const *self) {
  if (!self) {
    return;
  }

  printf("Cinema ID: %" PRIu16 "\n", self->id);
  for (int i = 0; i < 12; ++i) {
    prefixed_wstring_print(&self->strings[i]);
    printf("\n");
  }
}


size_t
cinema_hash(struct cinema const *self) {
  if (!self) {
    return 0;
  }

  size_t h = 0;

  h += self->id;

  for (int i = 0; i < 12; ++i) {
    h += prefixed_wstring_hash(&self->strings[i]);
  }

  return h;
}

size_t
cinema_size(struct cinema const *self) {
  if (!self) {
    return 0;
  }

  size_t s = sizeof(self->id);

  for (int i = 0; i < 12; ++i) {
    s += prefixed_wstring_size(&self->strings[i]);
  }

  return s;
}
