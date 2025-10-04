#include "rin-probe.h"

G_DEFINE_INTERFACE(RinProbe, rin_probe, G_TYPE_INTERFACE)

static
void
rin_probe_default_init(RinProbeInterface *iface) {
  iface->path = NULL;
  iface->project = NULL;
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
