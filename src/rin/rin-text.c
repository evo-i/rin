#include "rin-text.h"

struct _RinText {
  GObject parent_instance;
  gchar *original;
  gchar *value;
  gsize original_length;
  gsize value_length;
  gulong id;
  guint32 status;
  gchar *language;
};

G_DEFINE_TYPE(RinText, rin_text, G_TYPE_OBJECT)

static void
rin_text_finalize(GObject *object) {
  RinText *self = RIN_TEXT(object);
  g_clear_pointer(&self->original, g_free);
  g_clear_pointer(&self->value, g_free);
  g_clear_pointer(&self->language, g_free);
  G_OBJECT_CLASS(rin_text_parent_class)->finalize(object);
}

static void
rin_text_class_init(RinTextClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_text_finalize;
}

static void
rin_text_init(RinText *self) {
  self->original = NULL;
  self->value = NULL;
  self->original_length = 0;
  self->value_length = 0;
  self->id = 0;
  self->status = 0;
  self->language = NULL;
}

RinText *
rin_text_new(const gchar *text) {
  RinText *self = g_object_new(RIN_TYPE_TEXT, NULL);
  if (text) {
    self->original = g_strdup(text);
    self->original_length = g_utf8_strlen(text, -1);
    self->value = g_strdup(text);
    self->value_length = g_utf8_strlen(text, -1);
  } else {
    self->original = g_strdup("");
    self->original_length = 0;
    self->value = g_strdup("");
    self->value_length = 0;
  }

  self->id = 0;
  self->status = 0;
  self->language = g_strdup("en-US");

  return self;
}

void
rin_text_unref(RinText *self) {
  g_return_if_fail(RIN_IS_TEXT(self));
  g_object_unref(self);
}

gchar const *
rin_text_get_original(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), NULL);
  return self->original;
}

gchar const *
rin_text_get_value(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), NULL);
  return self->value;
}

gsize
rin_text_get_original_length(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), 0);
  return self->original_length;
}

gsize
rin_text_get_value_length(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), 0);
  return self->value_length;
}

gulong
rin_text_get_id(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), 0);
  return self->id;
}

guint32
rin_text_get_status(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), 0);
  return self->status;
}

gchar const *
rin_text_get_language(RinText *self) {
  g_return_val_if_fail(RIN_IS_TEXT(self), NULL);
  return self->language;
}

void
rin_text_set_original(RinText *self, const gchar *text) {
  g_return_if_fail(RIN_IS_TEXT(self));
  g_clear_pointer(&self->original, g_free);
  if (text) {
    self->original = g_strdup(text);
    self->original_length = g_utf8_strlen(text, -1);
  } else {
    self->original = g_strdup("");
    self->original_length = 0;
  }
}

void
rin_text_set_value(RinText *self, const gchar *text) {
  g_return_if_fail(RIN_IS_TEXT(self));
  g_clear_pointer(&self->value, g_free);
  if (text) {
    self->value = g_strdup(text);
    self->value_length = g_utf8_strlen(text, -1);
  } else {
    self->value = g_strdup("");
    self->value_length = 0;
  }
}

void
rin_text_set_id(RinText *self, gulong id) {
  g_return_if_fail(RIN_IS_TEXT(self));
  self->id = id;
}

void
rin_text_set_status(RinText *self, guint32 status) {
  g_return_if_fail(RIN_IS_TEXT(self));
  self->status = status;
}

void
rin_text_set_language(RinText *self, const gchar *language) {
  g_return_if_fail(RIN_IS_TEXT(self));
  g_clear_pointer(&self->language, g_free);
  if (language) {
    self->language = g_strdup(language);
  } else {
    self->language = g_strdup("en-US");
  }
}
