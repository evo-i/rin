/* rin-translation.h
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

#ifndef RIN_TRANSLATION_H
#define RIN_TRANSLATION_H 1

#include <glib-object.h>
#include "rin-translation-metadata.h"
#include "rin-exports.h"

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_TRANSLATION (rin_translation_get_type())

G_DECLARE_FINAL_TYPE(RinTranslation, rin_translation, RIN, TRANSLATION, GObject)
/**
 * rin_translation_new:
 *
 * Returns: (transfer full): a newly-allocated #RinTranslation
 */
RIN_API
RinTranslation *
rin_translation_new(void);

/**
 * rin_translation_get_metadata:
 * @self: a #RinTranslation
 *
 * Returns: (transfer none): the #RinTranslationMetadata owned by @self
 */
RIN_API
RinTranslationMetadata *
rin_translation_get_metadata(RinTranslation *self);

/**
 * rin_translation_get_strings:
 * @self: a #RinTranslation
 *
 * Returns: (transfer none) (element-type utf8): a #GList of strings owned by @self
 */
RIN_API
GList *
rin_translation_get_strings(RinTranslation *self);

/**
 * rin_translation_unref:
 * @self: a #RinTranslation
 *
 * Convenience wrapper for g_object_unref().
 */
RIN_API
void
rin_translation_unref(RinTranslation *self);

G_END_DECLS

#endif /* RIN_TRANSLATION_H */
