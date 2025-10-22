#ifndef RIN_REGISTRY_H
#define RIN_REGISTRY_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define RIN_TYPE_REGISTRY (rin_registry_get_type())

G_DECLARE_FINAL_TYPE(RinRegistry, rin_registry, RIN, REGISTRY, GObject)

RinRegistry *
rin_registry_new(void);

GList *
rin_registry_get_registered(RinRegistry *self);

gboolean
rin_registry_register(RinRegistry *self,
                      const gchar *name);

void
rin_registry_unref(RinRegistry *self);

G_END_DECLS

#endif /* !RIN_REGISTRY_H */
