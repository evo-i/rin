#include <gmodule.h>

#include "rin-plugin.h"

#define RIN_MODULE_PATH "plugins/librin-plugin-soul-worker.so"

#define RIN_MODULE_NEW "rin_plugin_entry"

typedef GTypeModule *(*rin_plugin_entry)(void);

int
main(int argc, char *argv[]) {
  GModule *g_module;
  GTypeModule *module;
  rin_plugin_entry rin_new;
  GType req_type;
  RinKit *obj;
  guint count_types;
  GType *types = NULL;

  g_module = g_module_open(RIN_MODULE_PATH, G_MODULE_BIND_LAZY);

  g_module_symbol(g_module, RIN_MODULE_NEW, (gpointer *) &rin_new);

  module = rin_new();

  g_type_module_use(module);

  types = g_type_children(G_TYPE_OBJECT, &count_types);
  req_type = G_TYPE_INVALID;

  for (guint i = 0; i < count_types; i++) {
    if (g_type_is_a(types[i], RIN_TYPE_KIT)) {
      gpointer type_ptr = g_type_get_plugin(types[i]);
      if (type_ptr == module) {
        req_type = types[i];
        break;
      }
    }
  }

  obj = RIN_KIT(g_object_new(req_type, NULL));

  rin_kit_save_project(obj, NULL, NULL);
  rin_kit_load_project(obj, NULL, NULL);
  rin_kit_create_project_from_path(obj, NULL);
  rin_kit_make_patch(obj, NULL, NULL);

  g_object_unref(obj);

  g_type_module_unuse(module);

  g_module_close(g_module);

  return 0;
}
