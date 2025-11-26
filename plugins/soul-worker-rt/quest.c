#include "quest.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct quest*
quest_new(void) {
  struct quest* self = malloc(sizeof(struct quest));
  if (!self)
    return NULL;
  memset(self, 0, sizeof(*self));
  return self;
}

size_t
quest_read(FILE* file, struct quest* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t title_read = prefixed_wstring_read(file, &self->title);
  VALIDATE_READ(title_read > 0, (void)0);

  if (!safe_size_add(total_size, title_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    return 0;
  }

  size_t text_read = prefixed_wstring_read(file, &self->text);
  if (text_read == 0) {
    prefixed_wstring_free(&self->title);
    return 0;
  }

  if (!safe_size_add(total_size, text_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    return 0;
  }

  size_t task_read = prefixed_wstring_read(file, &self->task);
  if (task_read == 0) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    return 0;
  }

  if (!safe_size_add(total_size, task_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    return 0;
  }

  size_t talk_read = prefixed_wstring_read(file, &self->talk);
  if (talk_read == 0) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    return 0;
  }

  if (!safe_size_add(total_size, talk_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    prefixed_wstring_free(&self->talk);
    return 0;
  }

  size_t dest_read = prefixed_wstring_read(file, &self->destination);
  if (dest_read == 0) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    prefixed_wstring_free(&self->talk);
    return 0;
  }

  if (!safe_size_add(total_size, dest_read, &total_size)) {
    prefixed_wstring_free(&self->title);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->task);
    prefixed_wstring_free(&self->talk);
    prefixed_wstring_free(&self->destination);
    return 0;
  }

  return total_size;
}

size_t
quest_write(FILE* file, struct quest const* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t title_write = prefixed_wstring_write(file, &self->title);
  VALIDATE_READ(title_write > 0, (void)0);

  if (!safe_size_add(total_size, title_write, &total_size)) {
    return 0;
  }

  size_t text_write = prefixed_wstring_write(file, &self->text);
  VALIDATE_READ(text_write > 0, (void)0);

  if (!safe_size_add(total_size, text_write, &total_size)) {
    return 0;
  }

  size_t task_write = prefixed_wstring_write(file, &self->task);
  VALIDATE_READ(task_write > 0, (void)0);

  if (!safe_size_add(total_size, task_write, &total_size)) {
    return 0;
  }

  size_t talk_write = prefixed_wstring_write(file, &self->talk);
  VALIDATE_READ(talk_write > 0, (void)0);

  if (!safe_size_add(total_size, talk_write, &total_size)) {
    return 0;
  }

  size_t dest_write = prefixed_wstring_write(file, &self->destination);
  VALIDATE_READ(dest_write > 0, (void)0);

  if (!safe_size_add(total_size, dest_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
quest_free(struct quest* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->title);
  prefixed_wstring_free(&self->text);
  prefixed_wstring_free(&self->task);
  prefixed_wstring_free(&self->talk);
  prefixed_wstring_free(&self->destination);
  free(self);
}

size_t
quest_hash(struct quest const* self) {
  size_t hash = 0;
  hash += self->id;
  hash += prefixed_wstring_hash(&self->title);
  hash += prefixed_wstring_hash(&self->text);
  hash += prefixed_wstring_hash(&self->task);
  hash += prefixed_wstring_hash(&self->talk);
  hash += prefixed_wstring_hash(&self->destination);
  return hash;
}

size_t
quest_size(struct quest const* self) {
  if (!self)
    return 0;
    
  return
    sizeof(self->id)
      + prefixed_wstring_size(&self->title)
      + prefixed_wstring_size(&self->text)
      + prefixed_wstring_size(&self->task)
      + prefixed_wstring_size(&self->talk)
      + prefixed_wstring_size(&self->destination);
}
