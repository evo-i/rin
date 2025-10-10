#ifndef RIN_MODULE_MANAGER_H
#define RIN_MODULE_MANAGER_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define RIN_TYPE_MODULE_MANAGER (rin_module_manager_get_type())

G_DECLARE_FINAL_TYPE(RinModuleManager,
  rin_module_manager,
  RIN, MODULE_MANAGER,
  GObject
)

RinModuleManager *
rin_module_manager_new(gchar const *module_path);

GPtrArray *
rin_module_manager_get_modules(RinModuleManager *self);

void
rin_module_manager_unref(RinModuleManager *self);

G_END_DECLS

#endif /* RIN_MODULE_MANAGER_H */
