#include "rin-kit.h"

G_DEFINE_INTERFACE(RinKit, rin_kit, G_TYPE_INTERFACE)

static
void
rin_kit_default_init(RinKitInterface *iface) {
  iface->get_probe_interface  = NULL;
  iface->create               = NULL;
  iface->open                 = NULL;
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

RinProject *
rin_kit_open(RinKit *self, gchar const *project_path) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), NULL);

  iface = RIN_KIT_GET_IFACE(self);

  return
    iface->open
      ? (iface->open)(self, project_path)
      : NULL;
}

RinProject *
rin_kit_create(RinKit *self, gchar const *program_path) {
  RinKitInterface *iface;
  g_return_val_if_fail(RIN_IS_KIT(self), NULL);

  iface = RIN_KIT_GET_IFACE(self);

  return
    iface->create
      ? (iface->create)(self, program_path)
      : NULL;
}
