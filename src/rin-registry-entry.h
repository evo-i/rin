#ifndef RIN_REGISTRY_ENTRY_H
#define RIN_REGISTRY_ENTRY_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define RIN_TYPE_REGISTRY_ENTRY (rin_registry_entry_get_type())

G_DECLARE_FINAL_TYPE(RinRegistryEntry, rin_registry_entry,
                     RIN, REGISTRY_ENTRY, GObject)

RinRegistryEntry *
rin_registry_entry_new(const gchar *path);

gchar const *
rin_registry_entry_get_name(RinRegistryEntry *self);

gchar const *
rin_registry_entry_get_path(RinRegistryEntry *self);

gchar const *
rin_registry_entry_get_version(RinRegistryEntry *self);

gchar const *
rin_registry_entry_get_author(RinRegistryEntry *self);

gboolean
rin_registry_entry_can_handle_file(RinRegistryEntry *self,
                                   gchar const *file_name);

gboolean
rin_registry_entry_can_handle_dir(RinRegistryEntry *self,
                                  gchar const *dir_name);

gboolean
rin_registry_entry_extract(RinRegistryEntry *self,
                           gchar const *src,
                           gchar const *dest);

gboolean
rin_registry_entry_build(RinRegistryEntry *self,
                         gchar const *src,
                         gchar const *dest);

gboolean
rin_registry_entry_is_valid(RinRegistryEntry *self);

void
rin_registry_entry_unref(RinRegistryEntry *self);

G_END_DECLS

#endif /* !RIN_REGISTRY_ENTRY_H */
