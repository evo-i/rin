/* rin-info.h
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

#ifndef RIN_INFO_H
#define RIN_INFO_H 1

#include <glib-object.h>
#include "rin-exports.h"

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_INFO (rin_info_get_type())
G_DECLARE_FINAL_TYPE(RinInfo, rin_info, RIN, INFO, GObject)
/**
 * rin_info_new:
 *
 * Returns: (transfer full): a newly-allocated #RinInfo
 */
RIN_API
RinInfo *
rin_info_new(void);

/**
 * rin_info_unref:
 * @self: a #RinInfo
 *
 * Convenience wrapper for g_object_unref().
 */
RIN_API
void
rin_info_unref(RinInfo *self);

/**
 * rin_info_get_name:
 * @self: a #RinInfo
 *
 * Returns: (transfer none) (nullable): the name string owned by @self
 */
RIN_API
gchar const *
rin_info_get_name(RinInfo *self);

/**
 * rin_info_get_version:
 * @self: a #RinInfo
 *
 * Returns: the version number stored in @self
 */
RIN_API
guint32
rin_info_get_version(RinInfo *self);

/**
 * rin_info_get_minimum_version:
 * @self: a #RinInfo
 *
 * Returns: the minimum supported version
 */
RIN_API
guint32
rin_info_get_minimum_version(RinInfo *self);

/**
 * rin_info_get_language:
 * @self: a #RinInfo
 *
 * Returns: (transfer none) (nullable): language string owned by @self
 */
RIN_API
gchar const *
rin_info_get_language(RinInfo *self);

/**
 * rin_info_set_name:
 * @self: a #RinInfo
 * @name: (nullable): name to set; copied by the setter
 */
RIN_API
void
rin_info_set_name(RinInfo *self, gchar const *name);

/**
 * rin_info_set_version:
 * @self: a #RinInfo
 * @version: version to set
 */
RIN_API
void
rin_info_set_version(RinInfo *self, guint32 version);

/**
 * rin_info_set_minimum_version:
 * @self: a #RinInfo
 * @min_version: minimum version to set
 */
RIN_API
void
rin_info_set_minimum_version(RinInfo *self, guint32 min_version);

/**
 * rin_info_set_language:
 * @self: a #RinInfo
 * @language: (nullable): language string; copied by the setter
 */
RIN_API
void
rin_info_set_language(RinInfo *self, gchar const *language);

G_END_DECLS

#endif // RIN_INFO_H
