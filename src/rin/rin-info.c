#include "rin-info.h"

struct _RinInfo {
  GObject parent_instance;

  gchar *name;
  guint32 version;
  guint32 minimum_version;
  gchar *language;
};

G_DEFINE_TYPE(RinInfo, rin_info, G_TYPE_OBJECT)

static void
rin_info_finalize(GObject *object) {
  RinInfo *self = RIN_INFO(object);
  g_clear_pointer(&self->name, g_free);
  g_clear_pointer(&self->language, g_free);
  G_OBJECT_CLASS(rin_info_parent_class)->finalize(object);
}

static void
rin_info_class_init(RinInfoClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_info_finalize;
}

static void
rin_info_init(RinInfo *self) {
  self->name = NULL;
  self->version = 0;
  self->minimum_version = 0;
  self->language = NULL;
}

RinInfo *
rin_info_new(void) {
  return g_object_new(RIN_TYPE_INFO, NULL);
}

void
rin_info_unref(RinInfo *self) {
  g_object_unref(self);
}

gchar const *
rin_info_get_name(RinInfo *self) {
  g_return_val_if_fail(RIN_IS_INFO(self), NULL);
  return self->name;
}

guint32
rin_info_get_version(RinInfo *self) {
  g_return_val_if_fail(RIN_IS_INFO(self), 0);
  return self->version;
}

guint32
rin_info_get_minimum_version(RinInfo *self) {
  g_return_val_if_fail(RIN_IS_INFO(self), 0);
  return self->minimum_version;
}

gchar const *
rin_info_get_language(RinInfo *self) {
  g_return_val_if_fail(RIN_IS_INFO(self), NULL);
  return self->language;
}

void
rin_info_set_name(RinInfo *self, gchar const *name) {
  g_return_if_fail(RIN_IS_INFO(self));
  g_return_if_fail(name != NULL);
  g_clear_pointer(&self->name, g_free);
  self->name = g_strdup(name);
}

void
rin_info_set_version(RinInfo *self, guint32 version) {
  g_return_if_fail(RIN_IS_INFO(self));
  self->version = version;
}

void
rin_info_set_minimum_version(RinInfo *self, guint32 min_version) {
  g_return_if_fail(RIN_IS_INFO(self));
  self->minimum_version = min_version;
}

void
rin_info_set_language(RinInfo *self, gchar const *language) {
  g_return_if_fail(RIN_IS_INFO(self));
  g_return_if_fail(language != NULL);
  g_clear_pointer(&self->language, g_free);
  self->language = g_strdup(language);
}
