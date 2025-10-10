#include "rin-plugin.h"

G_DEFINE_ABSTRACT_TYPE(RinPlugin, rin_plugin, G_TYPE_TYPE_MODULE)
G_DEFINE_INTERFACE(RinProbe, rin_probe, G_TYPE_OBJECT)
G_DEFINE_INTERFACE(RinKit, rin_kit, G_TYPE_OBJECT)

static
void
rin_plugin_init(RinPlugin *self) { }

static
void
rin_plugin_class_init(RinPluginClass *klass) { }

static
void
rin_probe_default_init(RinProbeInterface *klass) {
  klass->path = NULL;
  klass->project = NULL;

  klass->reserved1
    = klass->reserved2
    = klass->reserved3
    = klass->reserved4
      = NULL;
}

static
void
rin_kit_default_init(RinKitInterface *klass) {
  klass->create_project_from_path = NULL;
  klass->load_project = NULL;
  klass->save_project = NULL;
  klass->make_patch = NULL;

  klass->reserved1
    = klass->reserved2
    = klass->reserved3
    = klass->reserved4
      = NULL;
}

RinProject *
rin_kit_create_project_from_path(RinKit *self, gchar const *path) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), NULL);

  iface = RIN_KIT_GET_IFACE(self);

  return
    iface->create_project_from_path
      ? (iface->create_project_from_path)(self, path)
      : NULL;
}

gboolean
rin_kit_save_project(RinKit *self, RinProject *project, gchar const *path) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), FALSE);

  iface = RIN_KIT_GET_IFACE(self);

  return
    iface->save_project
      ? (iface->save_project)(self, project, path)
      : FALSE;
}

gboolean
rin_kit_load_project(RinKit *self, RinProject *project, gchar const *path) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), FALSE);

  iface = RIN_KIT_GET_IFACE(self);

  return
    iface->load_project
      ? (iface->load_project)(self, project, path)
      : FALSE;
}

gboolean
rin_kit_make_patch(RinKit *self, RinProject *project, gchar const *path) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), FALSE);

  iface = RIN_KIT_GET_IFACE(self);

  return
    iface->make_patch
      ? (iface->make_patch)(self, project, path)
      : FALSE;
}

gboolean
rin_probe_path(RinProbe *self, gchar const *path) {
  RinProbeInterface *iface;
  g_return_val_if_fail(RIN_IS_PROBE(self), FALSE);

  iface = RIN_PROBE_GET_IFACE(self);

  return
    iface->path
      ? (iface->path)(self, path)
      : FALSE;
}

gboolean
rin_probe_project(RinProbe *self, RinProject *project) {
  RinProbeInterface *iface;
  g_return_val_if_fail(RIN_IS_PROBE(self), FALSE);

  iface = RIN_PROBE_GET_IFACE(self);

  return
    iface->project
      ? (iface->project)(self, project)
      : FALSE;
}
