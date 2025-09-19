#include "rin-translation-metadata.h"

struct _RinTranslationMetadata {
  GObject parent_instance;

  gchar *language;
  gchar *author;
  gchar *version;
};

G_DEFINE_TYPE(RinTranslationMetadata, rin_translation_metadata, G_TYPE_OBJECT)

static void
rin_translation_metadata_finalize(GObject *object) {
  RinTranslationMetadata *self = RIN_TRANSLATION_METADATA(object);
  g_clear_pointer(&self->language, g_free);
  g_clear_pointer(&self->author, g_free);
  g_clear_pointer(&self->version, g_free);
  G_OBJECT_CLASS(rin_translation_metadata_parent_class)->finalize(object);
}

static void
rin_translation_metadata_class_init(RinTranslationMetadataClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_translation_metadata_finalize;
}

static void
rin_translation_metadata_init(RinTranslationMetadata *self) {
  self->language = NULL;
  self->author = NULL;
  self->version = NULL;
}

RinTranslationMetadata *
rin_translation_metadata_new(const gchar *language,
                          const gchar *author,
                          const gchar *version) {
  RinTranslationMetadata *self = g_object_new(RIN_TYPE_TRANSLATION_METADATA, NULL);
  if (language) {
    self->language = g_strdup(language);
  } else {
    self->language = g_strdup("en-US");
  }

  if (author) {
    self->author = g_strdup(author);
  } else {
    self->author = g_strdup("Unnamed");
  }

  if (version) {
    self->version = g_strdup(version);
  } else {
    self->version = g_strdup("1.0.0");
  }

  return self;
}

const gchar *
rin_translation_metadata_get_language(RinTranslationMetadata *self) {
  g_return_val_if_fail(RIN_IS_TRANSLATION_METADATA(self), NULL);
  return self->language;
}

const gchar *
rin_translation_metadata_get_author(RinTranslationMetadata *self) {
  g_return_val_if_fail(RIN_IS_TRANSLATION_METADATA(self), NULL);
  return self->author;
}

const gchar *
rin_translation_metadata_get_version(RinTranslationMetadata *self) {
  g_return_val_if_fail(RIN_IS_TRANSLATION_METADATA(self), NULL);
  return self->version;
}

/* vim: set et sw=2 ts=2: */
