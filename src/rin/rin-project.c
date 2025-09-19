#include "rin-project.h"
#include "rin-version.h"
#include "rin-info.h"

struct _RinProject {
  GObject parent_instance;

  RinMetadata *metadata;
  RinSettings *settings;
  RinInfo *info;
  RinStats *stats;
  GPtrArray *archives;
  gchar *path;
};

G_DEFINE_TYPE(RinProject, rin_project, G_TYPE_OBJECT)

static void
rin_project_finalize(GObject *object) {
  RinProject *self = RIN_PROJECT(object);
  g_clear_object(&self->metadata);
  g_clear_object(&self->settings);
  g_clear_object(&self->info);
  g_clear_object(&self->stats);

  if (self->archives) {
    for (guint i = 0; i < self->archives->len; i++) {
      g_free(g_ptr_array_index(self->archives, i));
    }

    g_ptr_array_free(self->archives, TRUE);
  }

  g_clear_pointer(&self->path, g_free);
  G_OBJECT_CLASS(rin_project_parent_class)->finalize(object);
}

static void
rin_project_class_init(RinProjectClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_project_finalize;
}

static void
rin_project_init(RinProject *self) {
  self->metadata = rin_metadata_new();
  self->settings = rin_settings_new();
  self->info = rin_info_new();
  self->stats = rin_stats_new();
  self->archives = g_ptr_array_new_with_free_func(g_free);
  self->path = NULL;
}

RinProject *
rin_project_new_empty(GError **error) {
  RinProject *self = g_object_new(RIN_TYPE_PROJECT, NULL);
  RinMetadata *metadata;
  RinSettings *settings;
  RinInfo *info;

  if (!self->metadata
      || !self->settings
      || !self->info
      || !self->stats
      || !self->archives) {
    g_set_error(error, G_FILE_ERROR, G_FILE_ERROR_NOSPC,
                "Failed to allocate RinProject components");
    g_clear_object(&self);
    return NULL;
  }

  metadata = rin_project_get_metadata(self);
  rin_metadata_set_author(metadata, "Unknown");
  rin_metadata_set_description(metadata, "No description");
  rin_metadata_set_name(metadata, "Untitled Project");
  rin_metadata_set_version(metadata, RIN_ENCODE_VERSION(1, 0, 0));
  rin_metadata_set_created(metadata, g_date_time_new_now_local());

  settings = rin_project_get_settings(self);
  rin_settings_set_encoding(settings, "UTF-8");
  rin_settings_set_max_text_length(settings, 65000);
  rin_settings_set_source_language(settings, "en-US");
  rin_settings_set_target_language(settings, "en-US");

  info = rin_project_get_info(self);
  rin_info_set_name(info, "Unknown Game");
  rin_info_set_version(info, RIN_ENCODE_VERSION(0, 0, 0));
  rin_info_set_minimum_version(info, RIN_ENCODE_VERSION(0, 0, 0));
  rin_info_set_language(info, "en-US");

  return self;
}

RinProject *
rin_project_open(gchar const *path, GError **error) {
  RinProject *self = rin_project_new_empty(error);
  g_return_val_if_fail(path != NULL, NULL);

  if (!self) {
    return NULL;
  }

  // Here you would add code to actually open and read the project file
  // For now, we just set the path
  self->path = g_strdup(path);
  return self;
}

RinMetadata *
rin_project_get_metadata(RinProject *self) {
  g_return_val_if_fail(RIN_IS_PROJECT(self), NULL);
  return self->metadata;
}

RinSettings *
rin_project_get_settings(RinProject *self) {
  g_return_val_if_fail(RIN_IS_PROJECT(self), NULL);
  return self->settings;
}

RinInfo *
rin_project_get_info(RinProject *self) {
  g_return_val_if_fail(RIN_IS_PROJECT(self), NULL);
  return self->info;
}

RinStats *
rin_project_get_stats(RinProject *self) {
  g_return_val_if_fail(RIN_IS_PROJECT(self), NULL);
  return self->stats;
}

GPtrArray *
rin_project_get_archives(RinProject *self) {
  g_return_val_if_fail(RIN_IS_PROJECT(self), NULL);
  return self->archives;
}

gchar const *
rin_project_get_path(RinProject *self) {
  g_return_val_if_fail(RIN_IS_PROJECT(self), NULL);
  return self->path;
}

void
rin_project_unref(RinProject *self) {
  g_return_if_fail(RIN_IS_PROJECT(self));
  g_object_unref(self);
}
