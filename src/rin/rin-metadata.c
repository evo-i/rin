#include "rin-metadata.h"

struct _RinMetadata {
  GObject parent_instance;

  gchar *name;
  gchar *description;
  gchar *author;
  guint32 version;
  GDateTime *created;
  GDateTime *modified;
};

G_DEFINE_TYPE(RinMetadata, rin_metadata, G_TYPE_OBJECT)

static void
rin_metadata_finalize(GObject *object) {
  RinMetadata *self = RIN_METADATA(object);

  g_clear_pointer(&self->name, g_free);
  g_clear_pointer(&self->description, g_free);
  g_clear_pointer(&self->author, g_free);
  g_clear_pointer(&self->created, g_date_time_unref);
  g_clear_pointer(&self->modified, g_date_time_unref);
  G_OBJECT_CLASS(rin_metadata_parent_class)->finalize(object);
}

static void
rin_metadata_class_init(RinMetadataClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_metadata_finalize;
}

static void
rin_metadata_init(RinMetadata *self) {
  self->name = NULL;
  self->description = NULL;
  self->author = NULL;
  self->version = 0;
  self->created = NULL;
  self->modified = NULL;
}

RinMetadata *
rin_metadata_new(void) {
  return g_object_new(RIN_TYPE_METADATA, NULL);
}

void rin_metadata_unref(RinMetadata *self) {
  g_object_unref(self);
}

gchar const *
rin_metadata_get_name(RinMetadata *self) {
  g_return_val_if_fail(RIN_IS_METADATA(self), NULL);
  return self->name;
}

gchar const *
rin_metadata_get_description(RinMetadata *self) {
  g_return_val_if_fail(RIN_IS_METADATA(self), NULL);
  return self->description;
}

gchar const *
rin_metadata_get_author(RinMetadata *self) {
  g_return_val_if_fail(RIN_IS_METADATA(self), NULL);
  return self->author;
}

guint32
rin_metadata_get_version(RinMetadata *self) {
  g_return_val_if_fail(RIN_IS_METADATA(self), 0);
  return self->version;
}

GDateTime *
rin_metadata_get_created(RinMetadata *self) {
  g_return_val_if_fail(RIN_IS_METADATA(self), NULL);
  return self->created;
}

GDateTime *
rin_metadata_get_modified(RinMetadata *self) {
  g_return_val_if_fail(RIN_IS_METADATA(self), NULL);
  return self->modified;
}

void rin_metadata_set_name(RinMetadata *self, gchar const *name) {
  g_return_if_fail(RIN_IS_METADATA(self));
  g_clear_pointer(&self->name, g_free);
  if (name) {
    self->name = g_strdup(name);
  } else {
    self->name = g_strdup("");
  }
}

void rin_metadata_set_description(RinMetadata *self, gchar const *description) {
  g_return_if_fail(RIN_IS_METADATA(self));
  g_clear_pointer(&self->description, g_free);
  if (description) {
    self->description = g_strdup(description);
  } else {
    self->description = g_strdup("");
  }
}

void rin_metadata_set_author(RinMetadata *self, gchar const *author) {
  g_return_if_fail(RIN_IS_METADATA(self));
  g_clear_pointer(&self->author, g_free);
  if (author) {
    self->author = g_strdup(author);
  }
  else {
    self->author = g_strdup("");
  }
}

void rin_metadata_set_version(RinMetadata *self, guint32 version) {
  g_return_if_fail(RIN_IS_METADATA(self));
  self->version = version;
}

void rin_metadata_set_created(RinMetadata *self, GDateTime *created) {
  g_return_if_fail(RIN_IS_METADATA(self));

  if (created) {
    g_clear_pointer(&self->created, g_date_time_unref);
    self->created = g_date_time_ref(created);
  }
}

void rin_metadata_set_modified(RinMetadata *self, GDateTime *modified) {
  g_return_if_fail(RIN_IS_METADATA(self));

  if (modified) {
    g_clear_pointer(&self->modified, g_date_time_unref);
    self->modified = g_date_time_ref(modified);
  }
}
