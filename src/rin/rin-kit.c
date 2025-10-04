#include "rin-kit.h"

G_DEFINE_INTERFACE(RinKit, rin_kit, G_TYPE_INTERFACE)

static
void
rin_kit_default_init(RinKitInterface *iface) {
  iface->get_probe_interface = NULL;
}

RinProbe *
rin_kit_get_probe_interface(RinKit *self) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), NULL);

  iface = RIN_KIT_GET_IFACE(self);
  
  return
    iface->get_probe_interface
      ? iface->get_probe_interface(self)
      : NULL;
}
