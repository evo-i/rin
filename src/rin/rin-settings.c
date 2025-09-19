#include "rin-settings.h"

typedef struct _RinSettingsPrivate {
  gchar *source_language;
  gchar *target_language;
  gchar *encoding;
  gsize max_text_length;
} RinSettingsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(RinSettings, rin_settings, G_TYPE_OBJECT)

static void
rin_settings_finalize(GObject *object) {
  RinSettings *self;
  RinSettingsPrivate *priv;

  self = RIN_SETTINGS(object);
  priv = rin_settings_get_instance_private(self);

  g_clear_pointer(&priv->source_language, g_free);
  g_clear_pointer(&priv->target_language, g_free);
  g_clear_pointer(&priv->encoding, g_free);
  G_OBJECT_CLASS(rin_settings_parent_class)->finalize(object);
}

static void
rin_settings_class_init(RinSettingsClass *klass) {
  GObjectClass *object_class;

  object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_settings_finalize;
}

static void
rin_settings_init(RinSettings *self) {
  RinSettingsPrivate *priv;

  priv = rin_settings_get_instance_private(self);
  priv->source_language = NULL;
  priv->target_language = NULL;
  priv->encoding = NULL;
  priv->max_text_length = 0;
}

RinSettings *
rin_settings_new(void) {
  return g_object_new(RIN_TYPE_SETTINGS, NULL);
}

gchar const *
rin_settings_get_source_language(RinSettings *self) {
  RinSettingsPrivate *priv;
  g_return_val_if_fail(RIN_IS_SETTINGS(self), NULL);
  priv = rin_settings_get_instance_private(self);
  return priv->source_language;
}

gchar const *
rin_settings_get_target_language(RinSettings *self) {
  RinSettingsPrivate *priv;
  g_return_val_if_fail(RIN_IS_SETTINGS(self), NULL);
  priv = rin_settings_get_instance_private(self);
  return priv->target_language;
}

gchar const *
rin_settings_get_encoding(RinSettings *self) {
  RinSettingsPrivate *priv;
  g_return_val_if_fail(RIN_IS_SETTINGS(self), NULL);
  priv = rin_settings_get_instance_private(self);
  return priv->encoding;
}

gsize
rin_settings_get_max_text_length(RinSettings *self) {
  RinSettingsPrivate *priv;
  g_return_val_if_fail(RIN_IS_SETTINGS(self), 0);
  priv = rin_settings_get_instance_private(self);
  return priv->max_text_length;
}

void
rin_settings_set_source_language(RinSettings *self, gchar const *language) {
  RinSettingsPrivate *priv;
  g_return_if_fail(RIN_IS_SETTINGS(self));
  priv = rin_settings_get_instance_private(self);
  g_clear_pointer(&priv->source_language, g_free);
  if (language) {
    priv->source_language = g_strdup(language);
  } else {
    priv->source_language = g_strdup("en-US");
  }
}

void
rin_settings_set_target_language(RinSettings *self, gchar const *language) {
  RinSettingsPrivate *priv;
  g_return_if_fail(RIN_IS_SETTINGS(self));
  priv = rin_settings_get_instance_private(self);
  g_clear_pointer(&priv->target_language, g_free);
  if (language) {
    priv->target_language = g_strdup(language);
  } else {
    priv->target_language = g_strdup("en-US");
  }
}

void
rin_settings_set_encoding(RinSettings *self, gchar const *encoding) {
  RinSettingsPrivate *priv;
  g_return_if_fail(RIN_IS_SETTINGS(self));
  priv = rin_settings_get_instance_private(self);
  g_clear_pointer(&priv->encoding, g_free);
  if (encoding) {
    priv->encoding = g_strdup(encoding);
  } else {
    priv->encoding = g_strdup("UTF-8");
  }
}

void
rin_settings_set_max_text_length(RinSettings *self, gsize max_length) {
  RinSettingsPrivate *priv;
  g_return_if_fail(RIN_IS_SETTINGS(self));
  priv = rin_settings_get_instance_private(self);
  priv->max_text_length = max_length;
}

void
rin_settings_unref(RinSettings *self) {
  g_return_if_fail(RIN_IS_SETTINGS(self));
  g_object_unref(self);
}
