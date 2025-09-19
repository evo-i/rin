/* rin-text.h
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

#ifndef RIN_TEXT_H
#define RIN_TEXT_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_TEXT (rin_text_get_type())
G_DECLARE_FINAL_TYPE(RinText, rin_text, RIN, TEXT, GObject)
/**
 * rin_text_new:
 * @text: (nullable): initial text copied by the constructor
 *
 * Returns: (transfer full): a newly-allocated #RinText
 */
RinText *
rin_text_new(const gchar *text);

/**
 * rin_text_unref:
 * @self: a #RinText
 *
 * Convenience wrapper for g_object_unref().
 */
void
rin_text_unref(RinText *self);

/**
 * rin_text_get_original:
 * @self: a #RinText
 *
 * Returns: (transfer none) (nullable): original text owned by @self
 */
gchar const *
rin_text_get_original(RinText *self);

/**
 * rin_text_get_value:
 * @self: a #RinText
 *
 * Returns: (transfer none) (nullable): current text value owned by @self
 */
gchar const *
rin_text_get_value(RinText *self);

/**
 * rin_text_get_original_length:
 * @self: a #RinText
 *
 * Returns: length of the original text
 */
gsize
rin_text_get_original_length(RinText *self);

/**
 * rin_text_get_value_length:
 * @self: a #RinText
 *
 * Returns: length of the current text value
 */
gsize
rin_text_get_value_length(RinText *self);

/**
 * rin_text_get_id:
 * @self: a #RinText
 *
 * Returns: numeric id
 */
gulong
rin_text_get_id(RinText *self);

/**
 * rin_text_get_status:
 * @self: a #RinText
 *
 * Returns: status flags
 */
guint32
rin_text_get_status(RinText *self);

/**
 * rin_text_get_language:
 * @self: a #RinText
 *
 * Returns: (transfer none) (nullable): language string owned by @self
 */
gchar const *
rin_text_get_language(RinText *self);

/**
 * rin_text_set_original:
 * @self: a #RinText
 * @text: (nullable): text copied by setter
 */
void
rin_text_set_original(RinText *self, const gchar *text);

/**
 * rin_text_set_value:
 * @self: a #RinText
 * @text: (nullable): text copied by setter
 */
void
rin_text_set_value(RinText *self, const gchar *text);

/**
 * rin_text_set_id:
 * @self: a #RinText
 * @id: id to set
 */
void
rin_text_set_id(RinText *self, gulong id);

/**
 * rin_text_set_status:
 * @self: a #RinText
 * @status: status flags to set
 */
void
rin_text_set_status(RinText *self, guint32 status);

/**
 * rin_text_set_language:
 * @self: a #RinText
 * @language: (nullable): language string copied by setter
 */
void
rin_text_set_language(RinText *self, const gchar *language);

G_END_DECLS

#endif /* RIN_TEXT_H */
