/* rin-metadata.h
 *
 * Copyright 2025 evo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef RIN_METADATA_H
#define RIN_METADATA_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_METADATA (rin_metadata_get_type())
G_DECLARE_FINAL_TYPE(RinMetadata, rin_metadata, RIN, METADATA, GObject)
/**
 * rin_metadata_new:
 * @returns: (transfer full): a newly-allocated #RinMetadata
 *
 * Create a new #RinMetadata instance.
 */
RinMetadata *
rin_metadata_new(void);

/**
 * rin_metadata_unref:
 * @self: a #RinMetadata
 *
 * Convenience wrapper for g_object_unref().
 */
void
rin_metadata_unref(RinMetadata *self);

/**
 * rin_metadata_get_name:
 * @self: a #RinMetadata
 *
 * Returns: (transfer none) (nullable): the name string owned by @self
 */
gchar const *
rin_metadata_get_name(RinMetadata *self);

/**
 * rin_metadata_get_description:
 * @self: a #RinMetadata
 *
 * Returns: (transfer none) (nullable): the description string owned by @self
 */
gchar const *
rin_metadata_get_description(RinMetadata *self);

/**
 * rin_metadata_get_author:
 * @self: a #RinMetadata
 *
 * Returns: (transfer none) (nullable): the author string owned by @self
 */
gchar const *
rin_metadata_get_author(RinMetadata *self);

/**
 * rin_metadata_get_version:
 * @self: a #RinMetadata
 *
 * Returns: the stored version number
 */
guint32
rin_metadata_get_version(RinMetadata *self);

/**
 * rin_metadata_get_created:
 * @self: a #RinMetadata
 *
 * Returns: (transfer none) (nullable): a #GDateTime representing creation time
 * owned by @self
 */
GDateTime *
rin_metadata_get_created(RinMetadata *self);

/**
 * rin_metadata_get_modified:
 * @self: a #RinMetadata
 *
 * Returns: (transfer none) (nullable): a #GDateTime representing last-modified
 * time owned by @self
 */
GDateTime *
rin_metadata_get_modified(RinMetadata *self);

/**
 * rin_metadata_set_name:
 * @self: a #RinMetadata
 * @name: (nullable): name to set; copied by the setter
 */
void
rin_metadata_set_name(RinMetadata *self, gchar const *name);

/**
 * rin_metadata_set_description:
 * @self: a #RinMetadata
 * @description: (nullable): description to set; copied by the setter
 */
void
rin_metadata_set_description(RinMetadata *self, gchar const *description);

/**
 * rin_metadata_set_author:
 * @self: a #RinMetadata
 * @author: (nullable): author to set; copied by the setter
 */
void
rin_metadata_set_author(RinMetadata *self, gchar const *author);

/**
 * rin_metadata_set_version:
 * @self: a #RinMetadata
 * @version: version to set
 */
void
rin_metadata_set_version(RinMetadata *self, guint32 version);

/**
 * rin_metadata_set_created:
 * @self: a #RinMetadata
 * @created: (nullable) (transfer none): a #GDateTime; the setter makes an internal copy
 */
void
rin_metadata_set_created(RinMetadata *self, GDateTime *created);

/**
 * rin_metadata_set_modified:
 * @self: a #RinMetadata
 * @modified: (nullable) (transfer none): a #GDateTime; the setter makes an internal copy
 */
void
rin_metadata_set_modified(RinMetadata *self, GDateTime *modified);

G_END_DECLS

#endif // RIN_METADATA_H
