#include "rin-archive.h"

struct _RinArchive {
  GObject parent_instance;

  gchar *path;
  GList *files;
};

G_DEFINE_TYPE(RinArchive, rin_archive, G_TYPE_OBJECT)

static void
rin_archive_finalize(GObject *object) {
  RinArchive *self = RIN_ARCHIVE(object);
  g_clear_pointer(&self->path, g_free);
  g_list_free_full(self->files, g_free);
  G_OBJECT_CLASS(rin_archive_parent_class)->finalize(object);
}

static void
rin_archive_class_init(RinArchiveClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_archive_finalize;
}

static void
rin_archive_init(RinArchive *self) {
  self->path = NULL;
  self->files = NULL;
}

RinArchive *
rin_archive_new(void) {
  return g_object_new(RIN_TYPE_ARCHIVE, NULL);
}

gchar const *
rin_archive_get_path(RinArchive *self) {
  g_return_val_if_fail(RIN_IS_ARCHIVE(self), NULL);
  return self->path;
}

GList *
rin_archive_get_files(RinArchive *self) {
  g_return_val_if_fail(RIN_IS_ARCHIVE(self), NULL);
  return self->files;
}

void
rin_archive_set_path(RinArchive *self, gchar const *path) {
  g_return_if_fail(RIN_IS_ARCHIVE(self));
  g_clear_pointer(&self->path, g_free);
  if (path)
    self->path = g_strdup(path);
}

void
rin_archive_unref(RinArchive *self) {
  g_object_unref(self);
}
