#define G_LOG_DOMAIN "RinPlugin-SoulWorker"
#define RIN_PLUGIN_SOUL_WORKER_NAME ("Rin Plugin SoulWorker")

#include "rin-plugin.h"

#define RIN_NOT_IMPLEMENTED() (g_warning("%s: Not implemented.", __func__))

#define RIN_TYPE_KIT_SOUL_WORKER (rin_kit_soul_worker_get_type())

G_DECLARE_FINAL_TYPE(RinKitSoulWorker,
  rin_kit_soul_worker,
  RIN, KIT_SOUL_WORKER,
  GObject)

struct _RinKitSoulWorker {
  GObject parent;
  RinProject *project;
  gchar *path;
  gboolean is_loaded;
};

static
RinProject *
rin_kit_soul_worker_create_project_from_path(RinKit *self, gchar const *path) {
  RIN_NOT_IMPLEMENTED();
  return NULL;
}

static
gboolean
rin_kit_soul_worker_load_project(RinKit *self, RinProject *project, gchar const *path) {
  RIN_NOT_IMPLEMENTED();
  return FALSE;
}

static
gboolean
rin_kit_soul_worker_save_project(RinKit *self, RinProject *project, gchar const *path) {
  RIN_NOT_IMPLEMENTED();
  return FALSE;
}

static
gboolean
rin_kit_soul_worker_make_patch(RinKit *self, RinProject *project, gchar const *path) {
  RIN_NOT_IMPLEMENTED();
  return FALSE;
}

static
void
rin_kit_soul_worker_interface_init(RinKitInterface *iface) {
  iface->create_project_from_path = rin_kit_soul_worker_create_project_from_path;
  iface->load_project = rin_kit_soul_worker_load_project;
  iface->save_project = rin_kit_soul_worker_save_project;
  iface->make_patch = rin_kit_soul_worker_make_patch;
}

G_DEFINE_DYNAMIC_TYPE_EXTENDED(RinKitSoulWorker,
  rin_kit_soul_worker,
  G_TYPE_OBJECT,
  G_TYPE_FLAG_FINAL,
  G_IMPLEMENT_INTERFACE_DYNAMIC(RIN_TYPE_KIT, rin_kit_soul_worker_interface_init)
)

static
void
rin_kit_soul_worker_init(RinKitSoulWorker *self) {
  (void) self;
}

static
void
rin_kit_soul_worker_finalize(GObject *object) {
  if (G_OBJECT_CLASS(rin_kit_soul_worker_parent_class)->finalize) {
    G_OBJECT_CLASS(rin_kit_soul_worker_parent_class)->finalize(object);
  }
}

static
void
rin_kit_soul_worker_class_init(RinKitSoulWorkerClass *klass) {
  (void) klass;
  klass->parent_class.finalize = rin_kit_soul_worker_finalize;
}

static
void
rin_kit_soul_worker_class_finalize(RinKitSoulWorkerClass *klass) {
  (void) klass;
}

#define RIN_TYPE_PLUGIN_SOUL_WORKER (rin_plugin_soul_worker_get_type())

static
gboolean
rin_plugin_soul_worker_load(GTypeModule *module) {
  g_warning("Plugin loaded.");
  rin_kit_soul_worker_register_type(module);
  return TRUE;
}

static
void
rin_plugin_soul_worker_unload(GTypeModule *module) {
  g_warning("Plugin unloaded.");
  (void) module;
}

RIN_DEFINE_PLUGIN(RinPluginSoulWorker,
  rin_plugin_soul_worker,
  RIN, PLUGIN_SOUL_WORKER,
  "RinPlugin SoulWorker")

// G_MODULE_EXPORT
// GTypeModule *
// rin_plugin_entry(void);

// G_DECLARE_FINAL_TYPE(RinPluginSoulWorker,
//   rin_plugin_soul_worker,
//   RIN, PLUGIN_SOUL_WORKER,
//   RinPlugin
// )

// struct _RinPluginSoulWorker {
//   RinPlugin parent;
// };

// G_DEFINE_FINAL_TYPE(RinPluginSoulWorker,
//   rin_plugin_soul_worker,
//   RIN_TYPE_PLUGIN
// )

// static void rin_plugin_soul_worker_class_init(RinPluginSoulWorkerClass *klass) {
//   GTypeModuleClass *module_class = G_TYPE_MODULE_CLASS(klass);
//   module_class->load = rin_plugin_soul_worker_load;
//   module_class->unload = rin_plugin_soul_worker_unload;
// }
// static void rin_plugin_soul_worker_init(RinPluginSoulWorker *self) {
//   (void) self;
// }

// G_MODULE_EXPORT GTypeModule *rin_plugin_entry(void) {
//   static GTypeModule *module = NULL;
//   if (module == NULL) {
//     module = g_object_new(rin_plugin_soul_worker_get_type(), NULL);
//     if (module) {
//       g_type_module_set_name(module, "RinPlugin SoulWorker");
//     }
//   }
//   return module;
// }
