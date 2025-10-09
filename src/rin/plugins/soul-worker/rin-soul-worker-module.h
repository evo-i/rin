#ifndef RIN_PLUGIN_SOUL_WORKER_MODULE_H
#define RIN_PLUGIN_SOUL_WORKER_MODULE_H 1

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define RIN_TYPE_PLUGIN_SOUL_WORKER_MODULE (rin_plugin_soul_worker_module_get_type())

G_DECLARE_FINAL_TYPE(RinPluginSoulWorkerModule,
  rin_plugin_soul_worker_module,
  RIN, PLUGIN_SOUL_WORKER_MODULE,
  GTypeModule
)

struct _RinPluginSoulWorkerModuleClass {
  GTypeModuleClass parent_class;
};

G_MODULE_EXPORT
GTypeModule *
rin_plugin_init(void);

G_END_DECLS

#endif /* RIN_PLUGIN_SOUL_WORKER_MODULE_H */
