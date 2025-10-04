#ifndef RIN_KIT_H
#define RIN_KIT_H 1

#include <glib-object.h>
#include "rin-probe.h"
#include "rin-exports.h"

G_BEGIN_DECLS

#define RIN_TYPE_KIT (rin_kit_get_type())

G_DECLARE_INTERFACE(RinKit, rin_kit, RIN, KIT, GObject)

struct _RinKitInterface {
  GTypeInterface parent_iface;

  RinProbe *
  (*get_probe_interface)(RinKit *self);
};

RIN_API
RinProbe *
rin_kit_get_probe_interface(RinKit *self);

G_END_DECLS

#endif /* RIN_KIT_H */
