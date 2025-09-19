#ifndef RIN_ARCHIVE_H
#define RIN_ARCHIVE_H 1

/* rin-archive.h
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

#include <glib-object.h>

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_ARCHIVE (rin_archive_get_type())
G_DECLARE_FINAL_TYPE(RinArchive, rin_archive, RIN, ARCHIVE, GObject)
/**
 * rin_archive_new:
 *
 * Returns: (transfer full): a newly-allocated #RinArchive
 */
RinArchive *
rin_archive_new(void);

/**
 * rin_archive_get_path:
 * @self: a #RinArchive
 *
 * Returns: (transfer none) (nullable): path string owned by @self
 */
gchar const *
rin_archive_get_path(RinArchive *self);

/**
 * rin_archive_get_files:
 * @self: a #RinArchive
 *
 * Returns: (transfer none) (element-type utf8): a #GList of UTF-8 file paths owned by @self
 */
GList *
rin_archive_get_files(RinArchive *self);

/**
 * rin_archive_set_path:
 * @self: a #RinArchive
 * @path: (nullable): path string copied by setter
 */
void
rin_archive_set_path(RinArchive *self, gchar const *path);

/**
 * rin_archive_unref:
 * @self: a #RinArchive
 *
 * Convenience wrapper for g_object_unref().
 */
void
rin_archive_unref(RinArchive *self);

G_END_DECLS

#endif /* RIN_ARCHIVE_H */
