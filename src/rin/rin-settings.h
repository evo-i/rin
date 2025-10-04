/* rin-settings.h
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

#ifndef RIN_SETTINGS_H
#define RIN_SETTINGS_H 1

#include <glib.h>
#include <glib-object.h>

#include "rin-exports.h"

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_SETTINGS (rin_settings_get_type())
G_DECLARE_DERIVABLE_TYPE(RinSettings, rin_settings, RIN, SETTINGS, GObject)

struct _RinSettingsClass {
  GObjectClass parent_class;

  gchar const *(*get_source_language)(RinSettings *self);
  gchar const *(*get_target_language)(RinSettings *self);
  gchar const *(*get_encoding)(RinSettings *self);
  gsize        (*get_max_text_length)(RinSettings *self);

  void (*set_source_language)(RinSettings *self, gchar const *language);
  void (*set_target_language)(RinSettings *self, gchar const *language);
  void (*set_encoding)(RinSettings *self, gchar const *encoding);
  void (*set_max_text_length)(RinSettings *self, gsize max_length);

  /* Padding for future expansion */
  gpointer padding[12];
};

/**
 * rin_settings_new:
 *
 * Returns: (transfer full): a newly-allocated #RinSettings
 */
RIN_API
RinSettings *
rin_settings_new(void);

/**
 * rin_settings_get_source_language:
 * @self: a #RinSettings
 *
 * Returns: (transfer none) (nullable): the source language string owned by @self
 */
RIN_API
gchar const *
rin_settings_get_source_language(RinSettings *self);

/**
 * rin_settings_get_target_language:
 * @self: a #RinSettings
 *
 * Returns: (transfer none) (nullable): the target language string owned by @self
 */
RIN_API
gchar const *
rin_settings_get_target_language(RinSettings *self);

/**
 * rin_settings_get_encoding:
 * @self: a #RinSettings
 *
 * Returns: (transfer none) (nullable): the encoding string owned by @self
 */
RIN_API
gchar const *
rin_settings_get_encoding(RinSettings *self);

/**
 * rin_settings_get_max_text_length:
 * @self: a #RinSettings
 *
 * Returns: maximum allowed text length
 */
RIN_API
gsize
rin_settings_get_max_text_length(RinSettings *self);

/**
 * rin_settings_set_source_language:
 * @self: a #RinSettings
 * @language: (nullable): language string copied by setter
 */
RIN_API
void
rin_settings_set_source_language(RinSettings *self, gchar const *language);

/**
 * rin_settings_set_target_language:
 * @self: a #RinSettings
 * @language: (nullable): language string copied by setter
 */
RIN_API
void
rin_settings_set_target_language(RinSettings *self, gchar const *language);

/**
 * rin_settings_set_encoding:
 * @self: a #RinSettings
 * @encoding: (nullable): encoding string copied by setter
 */
RIN_API
void
rin_settings_set_encoding(RinSettings *self, gchar const *encoding);

/**
 * rin_settings_set_max_text_length:
 * @self: a #RinSettings
 * @max_length: maximum allowed text length
 */
RIN_API
void
rin_settings_set_max_text_length(RinSettings *self, gsize max_length);

/**
 * rin_settings_unref:
 * @self: a #RinSettings
 *
 * Convenience wrapper for g_object_unref().
 */
RIN_API
void
rin_settings_unref(RinSettings *self);

G_END_DECLS

#endif // RIN_SETTINGS_H
