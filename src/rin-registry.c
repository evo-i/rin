#include "rin-registry.h"
#include "rin-registry-entry.h"
#include <gio/gio.h>

#if defined(_WIN32)
#  define PLUGIN_SUFFIX ".dll"
#elif defined(__APPLE__)
#  define PLUGIN_SUFFIX ".dylib"
#else
#  define PLUGIN_SUFFIX ".so"
#endif

enum {
  PROP_0,
  PROP_PLUGINS_DIR,
  PROP_PLUGINS_COUNT,
  PROP_LOADED_PLUGINS,
  PROP_INVALID_PLUGINS,
  /* Add properties here if needed in the future */
  N_PROPERTIES
};

enum {
  SIGNAL_PLUGIN_LOADED,
  SIGNAL_PLUGIN_UNLOADED,
  /* Add signals here if needed in the future */
  N_SIGNALS
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL };
static guint obj_signals[N_SIGNALS] = { 0 };

struct _RinRegistry {
  GObject parent_instance;

  GString *plugins_dir;
  GList *loaded_plugins;
  GList *invalid_plugins;
};

G_DEFINE_TYPE(RinRegistry, rin_registry, G_TYPE_OBJECT)

static void
rin_registry_scan_dir(RinRegistry *self, const gchar *dir_path) {
  GDir *dir = g_dir_open(dir_path, 0, NULL);
  if (!dir) {
    g_warning("Cannot open plugins directory: %s", dir_path);
    return;
  }

  const gchar *entry;
  while ((entry = g_dir_read_name(dir)) != NULL) {
    gchar *full_path = g_build_filename(dir_path, entry, NULL);

    if (g_file_test(full_path, G_FILE_TEST_IS_DIR)) {
      rin_registry_scan_dir(self, full_path);
      g_free(full_path);
      continue;
    }

    if (g_str_has_suffix(entry, PLUGIN_SUFFIX)) {
      RinRegistryEntry *plugin = rin_registry_entry_new(full_path);
      if (rin_registry_entry_is_valid(plugin)) {
        self->loaded_plugins = g_list_prepend(self->loaded_plugins, plugin);
        g_signal_emit(self, obj_signals[SIGNAL_PLUGIN_LOADED], 0, full_path);
      } else {
        self->invalid_plugins = g_list_prepend(self->invalid_plugins, plugin);
      }
    }

    g_free(full_path);
  }

  g_dir_close(dir);
}

static void
rin_registry_set_property(GObject *object,
                          guint property_id,
                          const GValue *value,
                          GParamSpec *pspec) {
  RinRegistry *self = RIN_REGISTRY(object);

  switch (property_id) {
    case PROP_PLUGINS_DIR: {
      gchar const *dir_path
        = g_value_get_pointer(value);

      if (!g_file_test(dir_path, G_FILE_TEST_IS_DIR
                                  | G_FILE_TEST_EXISTS)) {
        g_warning("Provided plugins directory path is invalid: %s", dir_path);
        return;
      }

      if (self->plugins_dir) {
        g_string_free(self->plugins_dir, TRUE);
      }
      self->plugins_dir = g_string_new(g_value_get_pointer(value));
    } break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static void
rin_registry_get_property(GObject *object,
                          guint property_id,
                          GValue *value,
                          GParamSpec *pspec) {
  RinRegistry *self = RIN_REGISTRY(object);

  switch (property_id) {
    case PROP_PLUGINS_DIR: {
      g_value_set_pointer(value,
                          self->plugins_dir ? self->plugins_dir->str : NULL);
    } break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static void
rin_registry_finalize(GObject *object) {
  RinRegistry *self = RIN_REGISTRY(object);

  g_string_free(self->plugins_dir, TRUE);
  g_list_free_full(self->loaded_plugins, g_object_unref);
  g_list_free_full(self->invalid_plugins, g_object_unref);

  G_OBJECT_CLASS(rin_registry_parent_class)->finalize(object);
}

static void
rin_registry_class_init(RinRegistryClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_registry_finalize;
  object_class->set_property = rin_registry_set_property;
  object_class->get_property = rin_registry_get_property;

  obj_properties[PROP_PLUGINS_DIR]
    = g_param_spec_pointer("plugins-dir",
                           "Plugins Directory",
                           "Directory path where plugins are located",
                           G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  obj_properties[PROP_PLUGINS_COUNT]
    = g_param_spec_uint64("plugins-count", "Plugins Count",
                          "Number of registered plugins",
                          0, G_MAXUINT64, 0,
                          G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

  obj_properties[PROP_LOADED_PLUGINS]
    = g_param_spec_pointer("loaded-plugins",
                           "Loaded Plugins",
                           "List of currently loaded plugins",
                           G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

  obj_properties[PROP_INVALID_PLUGINS]
    = g_param_spec_pointer("invalid-plugins",
                           "Invalid Plugins",
                           "List of plugins that failed to load",
                           G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

  obj_signals[SIGNAL_PLUGIN_LOADED]
    = g_signal_new("plugin-loaded",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                   NULL, G_TYPE_NONE, 1, G_TYPE_STRING);

  obj_signals[SIGNAL_PLUGIN_UNLOADED]
    = g_signal_new("plugin-unloaded",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                   NULL, G_TYPE_NONE, 1, G_TYPE_STRING);

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void
rin_registry_init(RinRegistry *self) {
  self->plugins_dir = NULL;
  self->loaded_plugins = NULL;
  self->invalid_plugins = NULL;
}

RinRegistry *
rin_registry_new(void) {
  gchar *current_dir
    = g_get_current_dir();
  gchar *plugins_path
    = g_build_path(G_DIR_SEPARATOR_S, current_dir, "plugins", NULL);

  RinRegistry *self
    = g_object_new(RIN_TYPE_REGISTRY, "plugins-dir", plugins_path, NULL);

  g_free(current_dir);
  g_free(plugins_path);

  return self;
}

gboolean
rin_registry_set_plugins_path(RinRegistry *self, gchar const *dir_path) {
  g_return_val_if_fail(RIN_IS_REGISTRY(self), FALSE);
  g_return_val_if_fail(dir_path != NULL, FALSE);

  if (!g_file_test(dir_path, G_FILE_TEST_IS_DIR
                              | G_FILE_TEST_EXISTS)) {
    g_warning("Provided plugins directory path is invalid: %s", dir_path);
    return FALSE;
  }

  g_object_set(self, "plugins-dir", dir_path, NULL);
  return TRUE;
}

gchar const *
rin_registry_get_plugins_path(RinRegistry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY(self), NULL);

  return
    self->plugins_dir
      ? self->plugins_dir->str
      : NULL;
}

gsize
rin_registry_get_entry_count(RinRegistry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY(self), 0);

  return g_list_length(self->loaded_plugins);
}

GList *
rin_registry_get_loaded(RinRegistry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY(self), NULL);

  return self->loaded_plugins;
}

gboolean
rin_registry_load(RinRegistry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY(self), FALSE);

  if (!self->plugins_dir) {
    g_warning("Plugins directory not set");
    return FALSE;
  }

  rin_registry_scan_dir(self, self->plugins_dir->str);
  return TRUE;
}

gboolean
rin_registry_reload(RinRegistry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY(self), FALSE);

  rin_registry_unload(self);
  return rin_registry_load(self);
}

void
rin_registry_unload(RinRegistry *self) {
  g_return_if_fail(RIN_IS_REGISTRY(self));

  g_list_free_full(self->loaded_plugins, g_object_unref);
  self->loaded_plugins = NULL;

  g_list_free_full(self->invalid_plugins, g_object_unref);
  self->invalid_plugins = NULL;
}

void
rin_registry_unref(RinRegistry *self) {
  g_return_if_fail(RIN_IS_REGISTRY(self));

  g_object_unref(self);
}
