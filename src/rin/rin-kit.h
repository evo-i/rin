#ifndef RIN_KIT_H
#define RIN_KIT_H 1

#define RIN_INSIDE

#include <glib-object.h>

#include "rin-probe.h"
#include "rin-exports.h"

G_BEGIN_DECLS

#define RIN_TYPE_KIT (rin_kit_get_type())

RIN_API
G_DECLARE_INTERFACE(RinKit, rin_kit, RIN, KIT, GObject)

struct _RinKitInterface {
  GTypeInterface parent_iface;

  RinProbe *
  (*get_probe_interface)(RinKit *self);

  RinProject *
  (*open)(RinKit *self, gchar const *project_path);

  RinProject *
  (*create)(RinKit *self, gchar const *program_path);
};

RIN_API
RinProbe *
rin_kit_get_probe_interface(RinKit *self);

RIN_API
RinProject *
rin_kit_open(RinKit *self, gchar const *project_path);

RIN_API
RinProject *
rin_kit_create(RinKit *self, gchar const *program_path);

G_END_DECLS

#undef RIN_INSIDE

#endif /* RIN_KIT_H */
