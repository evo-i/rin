#include "system_mail.h"
#include <stdlib.h>
#include <string.h>

struct system_mail *
system_mail_new(void) {
  struct system_mail *self = calloc(1, sizeof(struct system_mail));

  if (!self) {
    return NULL;
  }

  return self;
}

size_t
system_mail_read_from_file(FILE *file, struct system_mail *self) {
  size_t read_count = fread(&self->id, sizeof(self->id), 1, file);
  if (read_count != 1) return 0;

  read_count = fread(&self->unknown1, sizeof(self->unknown1), 1, file);
  if (read_count != 1) return 0;

  read_count = fread(&self->unknown2, sizeof(self->unknown2), 1, file);
  if (read_count != 1) return 0;

  size_t from_read = prefixed_wstring_read_from_file(file, &self->from);
  if (from_read == 0) return 0;

  size_t subject_read = prefixed_wstring_read_from_file(file, &self->subject);
  if (subject_read == 0) {
    prefixed_wstring_free(&self->from);
    return 0;
  }

  size_t text_read = prefixed_wstring_read_from_file(file, &self->text);
  if (text_read == 0) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    return 0;
  }

  size_t image_read = prefixed_wstring_read_from_file(file, &self->image);
  if (image_read == 0) {
    prefixed_wstring_free(&self->from);
    prefixed_wstring_free(&self->subject);
    prefixed_wstring_free(&self->text);
    return 0;
  }

  return sizeof(self->id) + sizeof(self->unknown1) + from_read + subject_read + text_read + image_read;
}

size_t
system_mail_write_to_file(FILE *file, struct system_mail const *self) {
  size_t write_count = 0;
  size_t id_write = fwrite(&self->id, sizeof(self->id), 1, file);
  if (id_write != 1) return 0;
  write_count += id_write * sizeof(self->id);

  size_t u1_write = fwrite(&self->unknown1, sizeof(self->unknown1), 1, file);
  if (u1_write != 1) return 0;
  write_count += u1_write * sizeof(self->unknown1);

  size_t u2_write = fwrite(&self->unknown2, sizeof(self->unknown2), 1, file);
  if (u2_write != 1) return 0;
  write_count += u2_write * sizeof(self->unknown2);

  size_t from_write = prefixed_wstring_write_to_file(file, &self->from);
  if (from_write == 0) return 0;
  write_count += from_write;

  size_t subject_write = prefixed_wstring_write_to_file(file, &self->subject);
  if (subject_write == 0) return 0;
  write_count += subject_write;

  size_t text_write = prefixed_wstring_write_to_file(file, &self->text);
  if (text_write == 0) return 0;
  write_count += text_write;

  size_t image_write = prefixed_wstring_write_to_file(file, &self->image);
  if (image_write == 0) return 0;
  write_count += image_write;

  return write_count;
}

size_t
system_mail_free(struct system_mail *self) {
  if (!self) return 0;
  prefixed_wstring_free(&self->from);
  prefixed_wstring_free(&self->subject);
  prefixed_wstring_free(&self->text);
  prefixed_wstring_free(&self->image);
  free(self);
  return 0;
}

size_t
system_mail_hash(struct system_mail const *self) {
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
system_mail_print(struct system_mail const *self) {
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
