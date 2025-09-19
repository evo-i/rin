/* rin-project.h
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

#ifndef RIN_PROJECT_H
#define RIN_PROJECT_H 1

#include <glib.h>
#include <glib-object.h>

#include "rin-metadata.h"
#include "rin-settings.h"
#include "rin-info.h"
#include "rin-stats.h"

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_PROJECT (rin_project_get_type())

G_DECLARE_FINAL_TYPE(RinProject, rin_project, RIN, PROJECT, GObject)

/*
 * Creation and destruction
 */
/**
 * rin_project_new:
 * @game_root_dir: (nullable): optional root directory for the game, or NULL
 * @error: return location for a #GError, or %NULL
 *
 * Creates and returns a new #RinProject. The caller takes ownership of the
 * returned object and must free it with g_object_unref() (or use
 * rin_project_unref()).
 *
 * Returns: (transfer full): a newly-allocated #RinProject
 */
RinProject *
rin_project_new(gchar const *game_root_dir, GError **error);

/**
 * rin_project_new_empty:
 * @error: return location for a #GError, or %NULL
 *
 * Create a new empty #RinProject with reasonable defaults. The caller owns
 * the returned object.
 *
 * Returns: (transfer full): a newly-allocated #RinProject
 */
RinProject *
rin_project_new_empty(GError **error);

/**
 * rin_project_open:
 * @path: (not nullable): path to the project file to open
 * @error: return location for a #GError, or %NULL
 *
 * Open and return a #RinProject for the given path. The caller owns the
 * returned object.
 *
 * Returns: (transfer full) (nullable): a newly-allocated #RinProject or %NULL on error
 */
RinProject *
rin_project_open(gchar const *path, GError **error);

/*
 * Accessors — all of these return pointers owned by the RinProject. The
 * caller must not free them. If the caller needs to keep an object beyond
 * the lifetime of the RinProject it should take its own reference.
 */
/**
 * rin_project_get_metadata:
 * @self: a #RinProject
 *
 * Returns: (transfer none): the #RinMetadata owned by @self
 */
RinMetadata *
rin_project_get_metadata(RinProject *self);

/**
 * rin_project_get_settings:
 * @self: a #RinProject
 *
 * Returns: (transfer none): the #RinSettings owned by @self
 */
RinSettings *
rin_project_get_settings(RinProject *self);

/**
 * rin_project_get_info:
 * @self: a #RinProject
 *
 * Returns: (transfer none): the #RinInfo owned by @self
 */
RinInfo *
rin_project_get_info(RinProject *self);

/**
 * rin_project_get_stats:
 * @self: a #RinProject
 *
 * Returns: (transfer none): the #RinStats owned by @self
 */
RinStats *
rin_project_get_stats(RinProject *self);

/**
 * rin_project_get_archives:
 * @self: a #RinProject
 *
 * Returns: (transfer none) (element-type utf8): a #GPtrArray of UTF-8
 * strings (file paths) owned by @self
 */
GPtrArray *
rin_project_get_archives(RinProject *self);

/**
 * rin_project_get_path:
 * @self: a #RinProject
 *
 * Returns: (transfer none) (nullable): the project path owned by @self
 */
gchar const *
rin_project_get_path(RinProject *self);

/**
 * rin_project_unref:
 * @self: a #RinProject
 *
 * Convenience wrapper for g_object_unref() on #RinProject instances.
 */
void
rin_project_unref(RinProject *self);

G_END_DECLS

#endif /* RIN_PROJECT_H */
