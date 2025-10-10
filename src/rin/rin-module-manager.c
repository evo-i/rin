#include "rin-module-manager.h"

struct _RinModuleManager {
  GObject parent_instance;
};

G_DEFINE_FINAL_TYPE(RinModuleManager, rin_module_manager, G_TYPE_OBJECT)

static
void
rin_module_manager_init(RinModuleManager *self) {
  (void) self;
}

static
void
rin_module_manager_class_init(RinModuleManagerClass *klass) {
  (void) klass;
}

RinModuleManager *
rin_module_manager_new(gchar const *module_path) {
  return g_object_new(RIN_TYPE_MODULE_MANAGER, NULL);
}

void
rin_module_manager_unref(RinModuleManager *self) {
  g_return_if_fail(RIN_IS_MODULE_MANAGER(self));

  g_object_unref(self);
}
