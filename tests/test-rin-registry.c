#include <glib.h>
#include "rin-registry.h"
#include "rin-registry-entry.h"

static void
test_registry_new(void) {
  RinRegistry *reg = rin_registry_new();
  g_assert_nonnull(reg);

  const gchar *path = rin_registry_get_plugins_path(reg);
  g_assert_nonnull(path);
  g_test_message("Default plugins path: %s", path);

  g_object_unref(reg);
}

static void
test_registry_set_path(void) {
  RinRegistry *reg = rin_registry_new();
  g_assert_nonnull(reg);

  gboolean ok = rin_registry_set_plugins_path(reg, ".");
  g_assert_true(ok);

  const gchar *path = rin_registry_get_plugins_path(reg);
  g_assert_cmpstr(path, ==, ".");

  g_object_unref(reg);
}

static void
test_registry_load_plugins(void) {
  RinRegistry *reg = rin_registry_new();
  g_assert_nonnull(reg);

  gboolean ok = rin_registry_set_plugins_path(reg, "./tests/plugins");
  g_assert_true(ok);

  ok = rin_registry_load(reg);
  g_assert_true(ok);

  gsize count = rin_registry_get_entry_count(reg);
  g_test_message("Loaded plugins: %lu", (unsigned long)count);

  g_assert_cmpuint(count, >, 0);

  rin_registry_unload(reg);
  g_object_unref(reg);
}

int
main(int argc, char *argv[]) {
  g_test_init(&argc, &argv, NULL);

  g_test_add_func("/rin/registry/new", test_registry_new);
  g_test_add_func("/rin/registry/set_path", test_registry_set_path);
  g_test_add_func("/rin/registry/load_plugins", test_registry_load_plugins);

  return g_test_run();
}
