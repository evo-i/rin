/* rin-translation-metadata.h
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

#ifndef RIN_TRANSLATION_METADATA_H
#define RIN_TRANSLATION_METADATA_H 1

#include <glib-object.h>
#include "rin-exports.h"

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_TRANSLATION_METADATA (rin_translation_metadata_get_type())

G_DECLARE_FINAL_TYPE(RinTranslationMetadata, rin_translation_metadata, RIN, TRANSLATION_METADATA, GObject)

/**
 * RinTranslationMetadata:
 *
 * Metadata for a translation entry.
 */

/**
 * rin_translation_metadata_new:
 * @language: (nullable): language string copied by the constructor
 * @author: (nullable): author string copied by the constructor
 * @version: (nullable): version string copied by the constructor
 *
 * Returns: (transfer full): a newly-allocated #RinTranslationMetadata
 */
RIN_API
RinTranslationMetadata *
rin_translation_metadata_new(const gchar *language,
                          const gchar *author,
                          const gchar *version);

/**
 * rin_translation_metadata_get_language:
 * @self: a #RinTranslationMetadata
 *
 * Returns: (transfer none) (nullable): language string owned by @self
 */
RIN_API
const gchar *
rin_translation_metadata_get_language(RinTranslationMetadata *self);

/**
 * rin_translation_metadata_get_author:
 * @self: a #RinTranslationMetadata
 *
 * Returns: (transfer none) (nullable): author string owned by @self
 */
RIN_API
const gchar *
rin_translation_metadata_get_author(RinTranslationMetadata *self);

/**
 * rin_translation_metadata_get_version:
 * @self: a #RinTranslationMetadata
 *
 * Returns: (transfer none) (nullable): version string owned by @self
 */
RIN_API
const gchar *
rin_translation_metadata_get_version(RinTranslationMetadata *self);

G_END_DECLS

#endif /* RIN_TRANSLATION_METADATA_H */
