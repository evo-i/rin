#include "quest.h"
#include <stdlib.h>
#include <string.h>

struct quest *
quest_new(void) {
  struct quest *self = malloc(sizeof(struct quest));
  if (!self) return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
quest_read_from_file(FILE *file, struct quest *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  size_t title_read = prefixed_wstring_read_from_file(file, &self->title);
  if (title_read == 0) return 0;

  size_t text_read = prefixed_wstring_read_from_file(file, &self->text);
  if (text_read == 0) {
    prefixed_wstring_free(&self->title);
    return 0;
  }

  size_t task_read = prefixed_wstring_read_from_file(file, &self->task);
  if (task_read == 0) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    return 0;
  }

  size_t talk_read = prefixed_wstring_read_from_file(file, &self->talk);
  if (talk_read == 0) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    return 0;
  }

  size_t dest_read = prefixed_wstring_read_from_file(file, &self->destination);
  if (dest_read == 0) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    prefixed_wstring_free(&self->talk);
    return 0;
  }

  return sizeof(self->id) + title_read + text_read + task_read + talk_read + dest_read;
}

size_t
quest_write_to_file(FILE *file, struct quest const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t title_write = prefixed_wstring_write_to_file(file, &self->title);
  if (title_write == 0) return 0;
  write_count += title_write;

  size_t text_write = prefixed_wstring_write_to_file(file, &self->text);
  if (text_write == 0) return 0;
  write_count += text_write;

  size_t task_write = prefixed_wstring_write_to_file(file, &self->task);
  if (task_write == 0) return 0;
  write_count += task_write;

  size_t talk_write = prefixed_wstring_write_to_file(file, &self->talk);
  if (talk_write == 0) return 0;
  write_count += talk_write;

  size_t dest_write = prefixed_wstring_write_to_file(file, &self->destination);
  if (dest_write == 0) return 0;
  write_count += dest_write;

  return write_count;
}

size_t
quest_free(struct quest *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->title);
  prefixed_wstring_free(&self->text);
  prefixed_wstring_free(&self->task);
  prefixed_wstring_free(&self->talk);
  prefixed_wstring_free(&self->destination);
  free(self);
  return 0;
}

size_t
quest_hash(struct quest const *self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->title);
  hash += prefixed_wstring_hash(&self->text);
  hash += prefixed_wstring_hash(&self->task);
  hash += prefixed_wstring_hash(&self->talk);
  hash += prefixed_wstring_hash(&self->destination);
  return hash;
}
