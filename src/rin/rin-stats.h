/* rin-stats.h
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

#ifndef RIN_STATS_H
#define RIN_STATS_H 1

#include <glib.h>
#include <glib-object.h>

#include "rin-exports.h"

G_BEGIN_DECLS

#if !defined(RIN_INSIDE) && !defined(RIN_COMPILATION)
# error "Only <rin.h> can be included directly."
#endif

#define RIN_TYPE_STATS (rin_stats_get_type())

RIN_API
G_DECLARE_FINAL_TYPE(RinStats, rin_stats, RIN, STATS, GObject)
/**
 * rin_stats_new:
 *
 * Returns: (transfer full): a newly-allocated #RinStats
 */
RIN_API
RinStats *
rin_stats_new(void);

/**
 * rin_stats_unref:
 * @self: a #RinStats
 *
 * Convenience wrapper for g_object_unref().
 */
RIN_API
void
rin_stats_unref(RinStats *self);

/**
 * rin_stats_get_total_strings:
 * @self: a #RinStats
 *
 * Returns: total number of strings
 */
RIN_API
gulong
rin_stats_get_total_strings(RinStats *self);

/**
 * rin_stats_get_translated_strings:
 * @self: a #RinStats
 *
 * Returns: number of translated strings
 */
RIN_API
gulong
rin_stats_get_translated_strings(RinStats *self);

/**
 * rin_stats_get_reviewed_strings:
 * @self: a #RinStats
 *
 * Returns: number of reviewed strings
 */
RIN_API
gulong
rin_stats_get_reviewed_strings(RinStats *self);

/**
 * rin_stats_get_proofread_strings:
 * @self: a #RinStats
 *
 * Returns: number of proofread strings
 */
RIN_API
gulong
rin_stats_get_proofread_strings(RinStats *self);

G_END_DECLS

#endif /* RIN_STATS_H */
