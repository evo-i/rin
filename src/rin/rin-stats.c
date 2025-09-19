#include "rin-stats.h"

struct _RinStats {
  GObject parent_instance;

  gulong total_strings;
  gulong translated_strings;
  gulong reviewed_strings;
  gulong proofread_strings;
};

G_DEFINE_TYPE(RinStats, rin_stats, G_TYPE_OBJECT)

static void
rin_stats_finalize(GObject *object) {
  G_OBJECT_CLASS(rin_stats_parent_class)->finalize(object);
}

static void
rin_stats_class_init(RinStatsClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = rin_stats_finalize;
}

static void
rin_stats_init(RinStats *self) {
  self->total_strings = 0;
  self->translated_strings = 0;
  self->reviewed_strings = 0;
  self->proofread_strings = 0;
}

RinStats *
rin_stats_new(void) {
  return g_object_new(RIN_TYPE_STATS, NULL);
}

void
rin_stats_unref(RinStats *self) {
  g_object_unref(self);
}

gulong
rin_stats_get_total_strings(RinStats *self) {
  g_return_val_if_fail(RIN_IS_STATS(self), 0);
  return self->total_strings;
}

gulong
rin_stats_get_translated_strings(RinStats *self) {
  g_return_val_if_fail(RIN_IS_STATS(self), 0);
  return self->translated_strings;
}

gulong
rin_stats_get_reviewed_strings(RinStats *self) {
  g_return_val_if_fail(RIN_IS_STATS(self), 0);
  return self->reviewed_strings;
}

gulong
rin_stats_get_proofread_strings(RinStats *self) {
  g_return_val_if_fail(RIN_IS_STATS(self), 0);
  return self->proofread_strings;
}
