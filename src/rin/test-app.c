#include <gmodule.h>

#include "rin-kit.h"

#define RIN_MODULE_PATH "C:/users/devbi/projects/rin/build/src/rin-sw/librin-plugin-sw.dll"

#define RIN_MODULE_NEW "rin_module_new"

typedef GTypeModule *(*rin_module_new)(void);

int
main(int argc, char *argv[]) {
  GModule *g_module;
  GTypeModule *module;

  g_module = g_module_open(RIN_MODULE_PATH, G_MODULE_BIND_LAZY);

  rin_module_new rin_new;

  g_module_symbol(g_module, RIN_MODULE_NEW, (gpointer *) &rin_new);

  module = rin_new();
  
  g_type_module_use(module);



  g_type_module_unuse(module);

  return 0;
}
