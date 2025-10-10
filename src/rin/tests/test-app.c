#include <gmodule.h>

#include "rin-plugin.h"

#define RIN_MODULE_PATH "plugins/librin-plugin-soul-worker.dll"

#define RIN_MODULE_NEW "rin_plugin_entry"

typedef GTypeModule *(*rin_plugin_entry)(void);

int
main(int argc, char *argv[]) {
  GModule *g_module;
  GTypeModule *module;
  rin_plugin_entry rin_new;
  GType req_type;
  GObject *obj;

  g_module = g_module_open(RIN_MODULE_PATH, G_MODULE_BIND_LAZY);

  g_module_symbol(g_module, RIN_MODULE_NEW, (gpointer *) &rin_new);

  module = rin_new();

  g_type_module_use(module);

  req_type = g_type_from_name("RinA");

  obj = g_object_new(req_type, NULL);

  gboolean state = g_type_is_a(req_type, RIN_TYPE_KIT);

  g_object_unref(obj);

  g_type_module_unuse(module);

  g_module_close(g_module);

  return 0;
}
