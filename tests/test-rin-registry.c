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

  gboolean ok = rin_registry_set_plugins_path(reg, "/home/evo/rin/build/plugins");
  g_assert_true(ok);

  ok = rin_registry_load(reg);
  g_assert_true(ok);

  gsize count = rin_registry_get_entry_count(reg);
  g_test_message("Loaded plugins: %lu", (unsigned long)count);

  g_assert_cmpuint(count, >, 0);

  rin_registry_unload(reg);
  g_object_unref(reg);
}

static void
test_registry_soul_worker(void) {
  RinRegistry *reg = rin_registry_new();
  RinRegistryEntry *entry;
  g_assert_nonnull(reg);

  gboolean ok = rin_registry_set_plugins_path(reg, "/home/evo/rin/build/plugins");
  g_assert_true(ok);

  ok = rin_registry_load(reg);
  g_assert_true(ok);

  GList *loaded = rin_registry_get_loaded(reg);
  gboolean found = FALSE;

  for (GList *l = loaded; l != NULL; l = l->next) {
    entry = (RinRegistryEntry *)l->data;
    const gchar *name = rin_registry_entry_get_name(entry);
    if (g_strcmp0(name, "Soul Worker Plugin") == 0) {
      found = TRUE;
      break;
    }
  }

  g_assert_true(found);

  gboolean status
    = rin_registry_entry_can_handle_dir(entry, "/mnt/f/SteamLibrary/steamapps/common/Soulworker_GB/");
  g_assert_true(status);

  status
    = rin_registry_entry_can_handle_dir(entry, "/mnt/f/SteamLibrary/steamapps/common/Soulworker_GB/datas/");

  g_assert_false(status);

  status = rin_registry_entry_can_handle_file(entry,
    "/mnt/f/SteamLibrary/steamapps/common/Soulworker_GB/datas/data11.v");

  g_assert_true(status);

  rin_registry_unload(reg);
  g_object_unref(reg);
}

static void
test_registry_extract(void) {
  RinRegistry *reg = rin_registry_new();
  RinRegistryEntry *entry;
  g_assert_nonnull(reg);

  gboolean ok = rin_registry_set_plugins_path(reg, "/home/evo/rin/build/plugins");
  g_assert_true(ok);

  ok = rin_registry_load(reg);
  g_assert_true(ok);

  GList *loaded = rin_registry_get_loaded(reg);
  gboolean found = FALSE;

  for (GList *l = loaded; l != NULL; l = l->next) {
    entry = (RinRegistryEntry *)l->data;
    const gchar *name = rin_registry_entry_get_name(entry);
    if (g_strcmp0(name, "Soul Worker Plugin") == 0) {
      found = TRUE;
      break;
    }
  }

  g_assert_true(found);

  gboolean status
    = rin_registry_entry_can_handle_file(entry,
      "/mnt/f/SteamLibrary/steamapps/common/Soulworker_GB/datas/data11.v");
  g_assert_true(status);

  status = rin_registry_entry_can_handle_dir(entry,
    "/mnt/f/SteamLibrary/steamapps/common/Soulworker_GB/");

  g_assert_true(status);

  rin_registry_entry_extract(entry,
    "/mnt/f/SteamLibrary/steamapps/common/Soulworker_GB/datas/",
    "./extracted");

  rin_registry_unload(reg);
  g_object_unref(reg);
}

int
main(int argc, char *argv[]) {
  g_test_init(&argc, &argv, NULL);

  g_test_add_func("/rin/registry/new", test_registry_new);
  g_test_add_func("/rin/registry/set_path", test_registry_set_path);
  g_test_add_func("/rin/registry/load_plugins", test_registry_load_plugins);
  g_test_add_func("/rin/registry/soul_worker", test_registry_soul_worker);
  g_test_add_func("/rin/registry/extract", test_registry_extract);

  return g_test_run();
}
