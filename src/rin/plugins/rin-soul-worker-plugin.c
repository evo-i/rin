#include "rin-plugin.h"
#include <glib-object.h>

#define G_LOG_ "RinPluginSoulWorker"

G_DECLARE_FINAL_TYPE(RinA, rin_a, RIN, A, GObject)

struct _RinA {
  GObject parent;
};

G_DEFINE_DYNAMIC_TYPE(RinA, rin_a, G_TYPE_OBJECT)

static
void
rin_a_init(RinA *self) {

}

static
void
rin_a_class_init(RinAClass *klass) {

}

static
void
rin_a_class_finalize(RinAClass *klass) {

}

RIN_DECLARE_PLUGIN

#define RIN_TYPE_PLUGIN_SOUL_WORKER (rin_plugin_soul_worker_get_type())

G_DECLARE_FINAL_TYPE(RinPluginSoulWorker, rin_plugin_soul_worker, RIN, PLUGIN_SOUL_WORKER, RinPlugin)

struct _RinPluginSoulWorker {
  RinPlugin parent;
};

struct _RinPluginSoulWorkerClass {
  RinPluginClass parent_class;
};

G_DEFINE_TYPE(RinPluginSoulWorker, rin_plugin_soul_worker, RIN_TYPE_PLUGIN)

static
gboolean
rin_plugin_soul_worker_load(GTypeModule *module) {
  rin_a_register_type(module);

  g_log(G_LOG_, G_LOG_LEVEL_DEBUG, "Module loaded...");

  g_type_add_interface_dynamic(rin_a_get_type(), RIN_TYPE_KIT, G_TYPE_PLUGIN(module));

  return TRUE;
}

static
void
rin_plugin_soul_worker_unload(GTypeModule *module) {
  g_log(G_LOG_, G_LOG_LEVEL_DEBUG, "Module unloaded...");
  (void) module;
}

static
void
rin_plugin_soul_worker_init(RinPluginSoulWorker *self) {
  (void) self;
}

static
void
rin_plugin_soul_worker_class_init(RinPluginSoulWorkerClass *klass) {
  GTypeModuleClass *module_class = G_TYPE_MODULE_CLASS(klass);
  module_class->load = rin_plugin_soul_worker_load;
  module_class->unload = rin_plugin_soul_worker_unload;
}

GTypeModule *
rin_plugin_entry(void) {
  RinPlugin *self;
  self = g_object_new(RIN_TYPE_PLUGIN_SOUL_WORKER, NULL);
  return G_TYPE_MODULE(self);
}

