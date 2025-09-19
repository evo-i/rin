#include "rin-translation.h"

struct _RinTranslation {
  GObject parent_instance;

  RinTranslationMetadata *metadata;
  GList *strings; // List of gchar* strings
};

G_DEFINE_TYPE(RinTranslation, rin_translation, G_TYPE_OBJECT)

static void
rin_translation_finalize(GObject *object) {
  RinTranslation *self = RIN_TRANSLATION(object);
  g_clear_object(&self->metadata);
  g_list_free_full(self->strings, g_object_unref);
  G_OBJECT_CLASS(rin_translation_parent_class)->finalize(object);
}

static void
rin_translation_class_init(RinTranslationClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_translation_finalize;
}

static void
rin_translation_init(RinTranslation *self) {
  self->metadata = NULL;
  self->strings = NULL;
}

RinTranslation *
rin_translation_new(void) {
  return g_object_new(RIN_TYPE_TRANSLATION, NULL);
}

RinTranslationMetadata *
rin_translation_get_metadata(RinTranslation *self) {
  g_return_val_if_fail(RIN_IS_TRANSLATION(self), NULL);
  return self->metadata;
}

GList *
rin_translation_get_strings(RinTranslation *self) {
  g_return_val_if_fail(RIN_IS_TRANSLATION(self), NULL);
  return self->strings;
}

void
rin_translation_unref(RinTranslation *self) {
  g_object_unref(self);
}
