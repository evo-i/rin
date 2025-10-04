#ifndef RIN_PROBE_H
#define RIN_PROBE_H 1

#include <glib-object.h>
#include "rin-project.h"
#include "rin-exports.h"

G_BEGIN_DECLS

#define RIN_TYPE_PROBE (rin_probe_get_type())

G_DECLARE_INTERFACE(RinProbe, rin_probe, RIN, PROBE, GObject)

struct _RinProbeInterface {
  GTypeInterface parent_iface;

  gboolean (*path)
  (RinProbe *self, gchar const *path);

  gboolean (*project)
  (RinProbe *self, RinProject *project);
};

RIN_API
gboolean
rin_probe_path(RinProbe *self, gchar const *path);

RIN_API
gboolean
rin_probe_project(RinProbe *self, RinProject *project);

G_END_DECLS

#endif /* RIN_PROBE_H */
