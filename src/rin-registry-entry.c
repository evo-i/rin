#include "rin-registry-entry.h"
#include "rin-plugin-api.h"

#include <gmodule.h>

struct _RinRegistryEntry {
  GObject parent_instance;

  gchar *path;
  GModule *module;

  rin_plugin_interface_t const *p_interface;
  rin_plugin_info_t const *p_info;
  rin_plugin_callbacks_t s_callbacks;
  void *p_instance;
  gboolean is_valid;
};

G_DEFINE_TYPE(RinRegistryEntry, rin_registry_entry, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_PATH,
  PROP_NAME,
  PROP_VERSION,
  PROP_AUTHOR,
  PROP_VALID,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL };

enum {
  SIGNAL_PLUGIN_REPORT,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = { 0 };

static void
rin_registry_entry_reportv(void *user_data,
                           rin_plugin_report_t level,
                           const char *msg,
                           float progress) {
  RinRegistryEntry *self = RIN_REGISTRY_ENTRY(user_data);
  gchar const *type = NULL;

  switch (level) {
    case rin_plugin_report_debug:
      type = "DEBUG";
      break;
    case rin_plugin_report_info:
      type = "INFO";
      break;
    case rin_plugin_report_warning:
      type = "WARNING";
      break;
    case rin_plugin_report_error:
      type = "ERROR";
      break;
    case rin_plugin_report_progress:
      type = "PROGRESS";
      break;
    default:
      type = "UNKNOWN";
      break;
  }

  if (level == rin_plugin_report_progress) {
    g_print("[0x%08" G_GINTPTR_MODIFIER "x|%s|%s] %s: %.2f%%\n",
            (gintptr) self, rin_registry_entry_get_name(self),
            type, msg, progress * 100.0f);
  } else {
    g_print("[0x%08" G_GINTPTR_MODIFIER "x|%s|%s] %s\n",
            (gintptr) self, rin_registry_entry_get_name(self), type, msg);
  }
}

static void
rin_registry_entry_dispose(GObject *object) {
  RinRegistryEntry *self = RIN_REGISTRY_ENTRY(object);

  if (self->p_interface && self->p_instance) {
    self->p_interface->destroy(self->p_instance);
    self->p_instance = NULL;
  }

  G_OBJECT_CLASS(rin_registry_entry_parent_class)->dispose(object);
}

static void
rin_registry_entry_finalize(GObject *object) {
  RinRegistryEntry *self = RIN_REGISTRY_ENTRY(object);

  g_clear_pointer(&self->path, g_free);

  if (self->module) {
    g_module_close(self->module);
    self->module = NULL;
  }

  G_OBJECT_CLASS(rin_registry_entry_parent_class)->finalize(object);
}

static void
rin_registry_entry_get_property(GObject *object,
                                guint property_id,
                                GValue *value,
                                GParamSpec *pspec) {
  RinRegistryEntry *self = RIN_REGISTRY_ENTRY(object);

  switch (property_id) {
    case PROP_PATH:
      g_value_set_string(value, self->path);
      break;
    case PROP_NAME:
      g_value_set_string(value, self->p_info ? self->p_info->psz_name : NULL);
      break;
    case PROP_VERSION:
      g_value_set_string(value, self->p_info ? self->p_info->psz_version : NULL);
      break;
    case PROP_AUTHOR:
      g_value_set_string(value, self->p_info ? self->p_info->psz_author : NULL);
      break;
    case PROP_VALID:
      g_value_set_boolean(value, self->is_valid);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }
}

static void
rin_registry_entry_set_property(GObject *object,
                                guint property_id,
                                const GValue *value,
                                GParamSpec *pspec) {
  RinRegistryEntry *self = RIN_REGISTRY_ENTRY(object);

  switch (property_id) {
    case PROP_PATH:
      g_free(self->path);
      self->path = g_value_dup_string(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }
}

static void
rin_registry_entry_class_init(RinRegistryEntryClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = rin_registry_entry_dispose;
  object_class->finalize = rin_registry_entry_finalize;
  object_class->get_property = rin_registry_entry_get_property;
  object_class->set_property = rin_registry_entry_set_property;

  obj_properties[PROP_PATH] =
    g_param_spec_string("path", "Path", "Path to plugin .so",
                        NULL, G_PARAM_READWRITE);

  obj_properties[PROP_NAME] =
    g_param_spec_string("name", "Name", "Plugin name",
                        NULL, G_PARAM_READABLE);

  obj_properties[PROP_VERSION] =
    g_param_spec_string("version", "Version", "Plugin version",
                        NULL, G_PARAM_READABLE);

  obj_properties[PROP_AUTHOR] =
    g_param_spec_string("author", "Author", "Plugin author",
                        NULL, G_PARAM_READABLE);

  obj_properties[PROP_VALID] =
    g_param_spec_boolean("valid", "Valid", "Whether plugin is valid",
                         FALSE, G_PARAM_READABLE);

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

  signals[SIGNAL_PLUGIN_REPORT] = g_signal_new(
    "plugin-report",
    G_TYPE_FROM_CLASS(klass),
    G_SIGNAL_RUN_LAST,
    0,
    NULL, NULL,
    g_cclosure_marshal_VOID__STRING,
    G_TYPE_NONE, 1, G_TYPE_STRING);
}

static void
rin_registry_entry_init(RinRegistryEntry *self) {
  self->is_valid = FALSE;
  self->module = NULL;
  self->path = NULL;
  self->p_interface = NULL;
  self->p_info = NULL;
  self->p_instance = NULL;

  self->s_callbacks.report = rin_registry_entry_reportv;
  self->s_callbacks.user_data = self;
}

RinRegistryEntry *
rin_registry_entry_new(const gchar *path) {
  RinRegistryEntry *self = g_object_new(RIN_TYPE_REGISTRY_ENTRY, NULL);
  self->path = g_strdup(path);

  /* Load the plugin module */
  GModule *module = g_module_open(path, G_MODULE_BIND_LOCAL);
  if (!module) {
    g_warning("Failed to load plugin module: %s", g_module_error());
    return self;
  }

  typedef rin_plugin_interface_t const *(*get_interface_func_t)(void);
  get_interface_func_t get_interface = NULL;
  gboolean have_sym =
    g_module_symbol(module,
                    RIN_PLUGIN_GET_INTERFACE_FUNC_NAME,
                    (gpointer *) &get_interface);
  if (!have_sym || !get_interface) {
    g_warning("Plugin module does not export required interface function");
    g_module_close(module);
    return self;
  }

  self->p_interface = get_interface();
  if (!self->p_interface) {
    g_warning("Failed to get plugin interface from module");
    g_module_close(module);
    return self;
  }

  self->p_info = self->p_interface->get_info();
  if (!self->p_info) {
    g_warning("Failed to get plugin info from interface");
    g_module_close(module);
    return self;
  }

  /* Create plugin instance */
  self->p_instance = self->p_interface->create(&self->s_callbacks);
  if (!self->p_instance) {
    g_warning("Failed to create plugin instance");
    g_module_close(module);
    return self;
  }

  self->module = module;
  self->is_valid = TRUE;
  return self;
}

gchar const *
rin_registry_entry_get_name(RinRegistryEntry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), NULL);
  return self->p_info ? self->p_info->psz_name : NULL;
}

gchar const *
rin_registry_entry_get_path(RinRegistryEntry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), NULL);
  return self->path;
}

gchar const *
rin_registry_entry_get_version(RinRegistryEntry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), NULL);
  return self->p_info ? self->p_info->psz_version : NULL;
}

gchar const *
rin_registry_entry_get_author(RinRegistryEntry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), NULL);
  return self->p_info ? self->p_info->psz_author : NULL;
}

gboolean
rin_registry_entry_can_handle_file(RinRegistryEntry *self,
                                   gchar const *file_name) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), FALSE);
  g_return_val_if_fail(file_name != NULL, FALSE);

  if (!self->is_valid) {
    return FALSE;
  }
  
  return self->p_interface->can_handle_file(self->p_instance, file_name);
}

gboolean
rin_registry_entry_can_handle_dir(RinRegistryEntry *self,
                                  gchar const *dir_name) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), FALSE);
  g_return_val_if_fail(dir_name != NULL, FALSE);

  if (!self->is_valid) return FALSE;
  return self->p_interface->can_handle_dir(self->p_instance, dir_name);
}

gboolean
rin_registry_entry_extract(RinRegistryEntry *self,
                           gchar const *src,
                           gchar const *dest) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), FALSE);
  g_return_val_if_fail(src != NULL, FALSE);
  g_return_val_if_fail(dest != NULL, FALSE);

  if (!self->is_valid) return FALSE;

  rin_plugin_error_t err =
    self->p_interface->extract(self->p_instance, src, dest);

  return err == rin_plugin_ok;
}

gboolean
rin_registry_entry_build(RinRegistryEntry *self,
                         gchar const *src,
                         gchar const *dest) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), FALSE);
  g_return_val_if_fail(src != NULL, FALSE);
  g_return_val_if_fail(dest != NULL, FALSE);

  if (!self->is_valid) return FALSE;

  rin_plugin_error_t err =
    self->p_interface->build(self->p_instance, src, dest);
  return err == rin_plugin_ok;
}

gboolean
rin_registry_entry_is_valid(RinRegistryEntry *self) {
  g_return_val_if_fail(RIN_IS_REGISTRY_ENTRY(self), FALSE);
  return self->is_valid;
}

void
rin_registry_entry_unref(RinRegistryEntry *self) {
  g_return_if_fail(RIN_IS_REGISTRY_ENTRY(self));
  g_object_unref(self);
}
