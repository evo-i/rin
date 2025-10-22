#ifndef RIN_REGISTRY_ENTRY_H
#define RIN_REGISTRY_ENTRY_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define RIN_TYPE_REGISTRY_ENTRY (rin_registry_entry_get_type())

G_DECLARE_FINAL_TYPE(RinRegistryEntry, rin_registry_entry,
                     RIN, REGISTRY_ENTRY, GObject)

RinRegistryEntry *
rin_registry_entry_new(const gchar *name);

const gchar *
rin_registry_entry_get_name(RinRegistryEntry *self);

void
rin_registry_entry_unref(RinRegistryEntry *self);

G_END_DECLS

#endif /* !RIN_REGISTRY_ENTRY_H */
