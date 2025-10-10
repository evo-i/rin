#ifndef RIN_PLUGIN_H
#define RIN_PLUGIN_H 1

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#include "rin.h"

#define RIN_TYPE_KIT (rin_kit_get_type())
#define RIN_TYPE_PROBE (rin_probe_get_type())
#define RIN_TYPE_PLUGIN (rin_plugin_get_type())

RIN_API
G_DECLARE_INTERFACE(RinKit, rin_kit, RIN, KIT, GObject)

RIN_API
G_DECLARE_INTERFACE(RinProbe, rin_probe, RIN, PROBE, GObject)

RIN_API
G_DECLARE_DERIVABLE_TYPE(RinPlugin, rin_plugin, RIN, PLUGIN, GTypeModule)

struct _RinPluginClass {
  GTypeModuleClass parent_class;

  /* reserved for future expansion and ABI compatability */
  gpointer reserved1;
  gpointer reserved2;
  gpointer reserved3;
  gpointer reserved4;
};

struct _RinProbeInterface {
  GTypeInterface parent_iface;

  /*< public >*/
  gboolean
  (*path)
  (RinProbe *self, gchar const *program_path);

  gboolean
  (*project)
  (RinProbe *self, RinProject *project);

  /* reserved for future expansion and ABI compatability */
  gpointer reserved1;
  gpointer reserved2;
  gpointer reserved3;
  gpointer reserved4;
};

struct _RinKitInterface {
  GTypeInterface parent_iface;

  /*< public >*/
  RinProject *
  (*create_project_from_path)
  (RinKit *self, gchar const *path);

  gboolean
  (*save_project)
  (RinKit *self, RinProject *project, gchar const *path);

  gboolean
  (*load_project)
  (RinKit *self, RinProject *project, gchar const *path);

  gboolean
  (*make_patch)
  (RinKit *self, RinProject *project, gchar const *path);

  /* reserved for future expansion and ABI compatability */
  gpointer reserved1;
  gpointer reserved2;
  gpointer reserved3;
  gpointer reserved4;
};

#define RIN_DEFINE_PLUGIN(ModuleObjName, module_obj_name, MODULE, OBJ_NAME, ModuleName)   \
G_MODULE_EXPORT GTypeModule *rin_plugin_entry(void);                                      \
G_DECLARE_FINAL_TYPE(ModuleObjName, module_obj_name, MODULE, OBJ_NAME, RinPlugin)         \
struct _##ModuleObjName {                                                                 \
  struct _RinPlugin parent;                                                               \
};                                                                                        \
G_DEFINE_TYPE(ModuleObjName, module_obj_name, RIN_TYPE_PLUGIN)                            \
static void module_obj_name##_class_init(ModuleObjName##Class *klass) {                   \
  GTypeModuleClass *module_class = G_TYPE_MODULE_CLASS(klass);                            \
  module_class->load = module_obj_name##_load;                                            \
  module_class->unload = module_obj_name##_unload;                                        \
}                                                                                         \
static void module_obj_name##_init(ModuleObjName *self) {                                 \
  (void) self;                                                                            \
}                                                                                         \
G_MODULE_EXPORT GTypeModule *rin_plugin_entry(void) {                                     \
  static GTypeModule *module = NULL;                                                      \
  if (module == NULL) {                                                                   \
    module = g_object_new(module_obj_name##_get_type(), NULL);                            \
    if (module) {                                                                         \
      g_type_module_set_name(module, ModuleName);                                         \
    }                                                                                     \
  }                                                                                       \
  return module;                                                                          \
}                                                                                         \

RIN_API
RinProject *
rin_kit_create_project_from_path(RinKit *self, gchar const *path);

RIN_API
gboolean
rin_kit_save_project(RinKit *self, RinProject *project, gchar const *path);

RIN_API
gboolean
rin_kit_load_project(RinKit *self, RinProject *project, gchar const *path);

RIN_API
gboolean
rin_kit_make_patch(RinKit *self, RinProject *project, gchar const *path);

RIN_API
gboolean
rin_probe_path(RinProbe *self, gchar const *path);

RIN_API
gboolean
rin_probe_project(RinProbe *self, RinProject *project);

G_END_DECLS

#endif /* RIN_PLUGIN_H */
