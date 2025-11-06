#ifndef RIN_REGISTRY_H
#define RIN_REGISTRY_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define RIN_TYPE_REGISTRY (rin_registry_get_type())

G_DECLARE_FINAL_TYPE(RinRegistry, rin_registry, RIN, REGISTRY, GObject)

RinRegistry *
rin_registry_new(void);

gboolean
rin_registry_set_plugins_path(RinRegistry *self, gchar const *dir_path);

gchar const *
rin_registry_get_plugins_path(RinRegistry *self);

gsize
rin_registry_get_entry_count(RinRegistry *self);

GList *
rin_registry_get_loaded(RinRegistry *self);

gboolean
rin_registry_load(RinRegistry *self);

gboolean
rin_registry_reload(RinRegistry *self);

void
rin_registry_unload(RinRegistry *self);

void
rin_registry_unref(RinRegistry *self);

G_END_DECLS

#endif /* !RIN_REGISTRY_H */
