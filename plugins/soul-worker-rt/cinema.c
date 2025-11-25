#include "cinema.h"

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
cinema_read_from_file(FILE *file, struct cinema *self) {
  size_t read_count = 0;

  read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) {
    return 0;
  }

  size_t total = sizeof(self->id);

  for (int i = 0; i < CINEMA_STRING_COUNT; ++i) {
    size_t n = prefixed_wstring_read_from_file(file, &self->strings[i]);
    if (n == 0) {
      for (int j = 0; j < i; ++j) {
        prefixed_wstring_free(&self->strings[j]);
      }
      return 0;
    }
    total += n;
  }

  return total;
}

size_t
cinema_write_to_file(FILE *file, struct cinema const *self) {
  size_t write_count = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) {
    return 0;
  }

  write_count += id_write;

  for (int i = 0; i < 12; ++i) {
    size_t w = prefixed_wstring_write_to_file(file, &self->strings[i]);
    if (w == 0) {
      return 0;
    }
    write_count += w;
  }

  return write_count;
}

size_t
cinema_free(struct cinema *self) {
  if (!self) {
    return 0;
  }

  for (int i = 0; i < 12; ++i) {
    prefixed_wstring_free(&self->strings[i]);
  }

  free(self);
  return 0;
}

size_t
cinema_print(struct cinema const *self) {
  if (!self) {
    return 0;
  }

  printf("Cinema ID: %" PRIu16 "\n", self->id);
  for (int i = 0; i < 12; ++i) {
    prefixed_wstring_print(&self->strings[i]);
    printf("\n");
  }

  return 0;
}

size_t
cinema_compare(struct cinema const *a,
               struct cinema const *b) {
  if (a == b) {
    return 1;
  }

  if (!a || !b) {
    return 0;
  }

  if (a->id != b->id) {
    return 0;
  }

  for (int i = 0; i < 12; ++i) {
    if (!prefixed_wstring_compare(&a->strings[i], &b->strings[i])) {
      return 0;
    }
  }

  return 1;
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

size_t
cinema_copy(struct cinema *dest,
            struct cinema const *src) {
  if (!dest || !src) {
    return 0;
  }

  dest->id = src->id;

  for (int i = 0; i < 12; ++i) {
    dest->strings[i].length = 0;
    dest->strings[i].value = NULL;

    if (prefixed_wstring_copy(&dest->strings[i], &src->strings[i]) == 0) {
      for (int j = 0; j < i; ++j) {
        prefixed_wstring_free(&dest->strings[j]);
      }
      return 0;
    }
  }

  return cinema_size(dest);
}
