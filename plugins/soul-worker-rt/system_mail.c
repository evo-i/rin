#include "system_mail.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

struct system_mail*
system_mail_new(void) {
  struct system_mail* self = calloc(1, sizeof(struct system_mail));

  if (!self) {
    return NULL;
  }

  return self;
}

size_t
system_mail_read(FILE* file, struct system_mail* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  read_count = fread(&self->unknown1, sizeof(self->unknown1), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown1), &total_size)) {
    return 0;
  }

  read_count = fread(&self->unknown2, sizeof(self->unknown2), 1, file);
  VALIDATE_READ(read_count == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown2), &total_size)) {
    return 0;
  }

  size_t from_read = prefixed_wstring_read(file, &self->from);
  VALIDATE_READ(from_read > 0, (void)0);

  if (!safe_size_add(total_size, from_read, &total_size)) {
    prefixed_wstring_free(&self->from);
    return 0;
  }

  size_t subject_read = prefixed_wstring_read(file, &self->subject);
  if (subject_read == 0) {
    prefixed_wstring_free(&self->from);
    return 0;
  }

  if (!safe_size_add(total_size, subject_read, &total_size)) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    return 0;
  }

  size_t text_read = prefixed_wstring_read(file, &self->text);
  if (text_read == 0) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    return 0;
  }

  if (!safe_size_add(total_size, text_read, &total_size)) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    prefixed_wstring_free(&self->text);
    return 0;
  }

  size_t image_read = prefixed_wstring_read(file, &self->image);
  if (image_read == 0) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    prefixed_wstring_free(&self->text);
    return 0;
  }

  if (!safe_size_add(total_size, image_read, &total_size)) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    prefixed_wstring_free(&self->text);
    prefixed_wstring_free(&self->image);
    return 0;
  }

  return total_size;
}

size_t
system_mail_write(FILE* file, struct system_mail const* self) {
  VALIDATE_PTR(file);
  VALIDATE_PTR(self);

  size_t total_size = 0;

  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  VALIDATE_READ(id_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->id), &total_size)) {
    return 0;
  }

  size_t u1_write = fwrite(&self->unknown1, sizeof(self->unknown1), 1, file);
  VALIDATE_READ(u1_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown1), &total_size)) {
    return 0;
  }

  size_t u2_write = fwrite(&self->unknown2, sizeof(self->unknown2), 1, file);
  VALIDATE_READ(u2_write == 1, (void)0);

  if (!safe_size_add(total_size, sizeof(self->unknown2), &total_size)) {
    return 0;
  }

  size_t from_write = prefixed_wstring_write(file, &self->from);
  VALIDATE_READ(from_write > 0, (void)0);

  if (!safe_size_add(total_size, from_write, &total_size)) {
    return 0;
  }

  size_t subject_write = prefixed_wstring_write(file, &self->subject);
  VALIDATE_READ(subject_write > 0, (void)0);

  if (!safe_size_add(total_size, subject_write, &total_size)) {
    return 0;
  }

  size_t text_write = prefixed_wstring_write(file, &self->text);
  VALIDATE_READ(text_write > 0, (void)0);

  if (!safe_size_add(total_size, text_write, &total_size)) {
    return 0;
  }

  size_t image_write = prefixed_wstring_write(file, &self->image);
  VALIDATE_READ(image_write > 0, (void)0);

  if (!safe_size_add(total_size, image_write, &total_size)) {
    return 0;
  }

  return total_size;
}

void
system_mail_free(struct system_mail* self) {
  if (!self)
    return;
  prefixed_wstring_free(&self->from);
  prefixed_wstring_free(&self->subject);
  prefixed_wstring_free(&self->text);
  prefixed_wstring_free(&self->image);
  free(self);
}

size_t
system_mail_hash(struct system_mail const* self) {
  size_t hash = 0;
  hash += self->id;
  hash += self->unknown1;
  hash += self->unknown2;

  hash += prefixed_wstring_hash(&self->from);
  hash += prefixed_wstring_hash(&self->subject);
  hash += prefixed_wstring_hash(&self->text);
  hash += prefixed_wstring_hash(&self->image);
  return hash;
}

void
system_mail_print(struct system_mail const* self) {
  if (!self) {
    printf("NULL system_mail\n");
    return;
  }

  printf("System Mail ID: %u\n", self->id);
  printf("Unknown1: %u\n", self->unknown1);
  printf("From: ");
  prefixed_wstring_print(&self->from);
  printf("\nSubject: ");
  prefixed_wstring_print(&self->subject);
  printf("\nText: ");
  prefixed_wstring_print(&self->text);
  printf("\nImage: ");
  prefixed_wstring_print(&self->image);
  printf("\n");
}

size_t
system_mail_size(struct system_mail const* self) {
  if (!self)
    return 0;

  size_t size = 0;
  size += sizeof(self->id);
  size += sizeof(self->unknown1);
  size += sizeof(self->unknown2);
  size += prefixed_wstring_size(&self->from);
  size += prefixed_wstring_size(&self->subject);
  size += prefixed_wstring_size(&self->text);
  size += prefixed_wstring_size(&self->image);
  return size;
}
