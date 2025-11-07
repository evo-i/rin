#include <glib.h>
#include <glib/gstdio.h>
#include <glib/goption.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rin-registry.h"
#include "rin-registry-entry.h"

static gchar **game_folder = NULL;
static gchar *output_dir = NULL;

static GOptionEntry entries[] = {
  { "output", 'o', 0, G_OPTION_ARG_FILENAME, &output_dir, "Output directory for extracted files", "DIR" },
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &game_folder, "Game folder path", "DIR" },
  { NULL }
};

int
main(int argc, char *argv[]) {
  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new("- extract Soul Worker archives");
  g_option_context_add_main_entries(context, entries, NULL);
  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    fprintf(stderr, "Option parsing failed: %s\n", error->message);
    g_error_free(error);
    g_option_context_free(context);
    return 1;
  }

  if (!game_folder || !game_folder[0]) {
    fprintf(stderr, "Usage: %s [OPTIONS] <game_folder_path>\n", argv[0]);
    fprintf(stderr, "Run '%s --help' for more information.\n", argv[0]);
    g_option_context_free(context);
    return 1;
  }

  // Now game_folder is set
  const char *gf = game_folder[0];

  // Check if gf exists and is a directory
  if (!g_file_test(gf, G_FILE_TEST_IS_DIR)) {
    fprintf(stderr, "Error: %s is not a valid directory\n", gf);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  // Initialize rin registry
  RinRegistry *reg = rin_registry_new();
  if (!reg) {
    fprintf(stderr, "Failed to create rin registry\n");
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  // Set plugins path to plugins
  char *plugins_path = g_build_filename(g_get_current_dir(), "plugins", NULL);
  if (!rin_registry_set_plugins_path(reg, plugins_path)) {
    fprintf(stderr, "Failed to set plugins path to %s\n", plugins_path);
    g_free(plugins_path);
    g_object_unref(reg);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }
  g_free(plugins_path);

  // Load plugins
  if (!rin_registry_load(reg)) {
    fprintf(stderr, "Failed to load plugins\n");
    g_object_unref(reg);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  // Find soul-worker plugin
  GList *loaded = rin_registry_get_loaded(reg);
  RinRegistryEntry *entry = NULL;
  for (GList *l = loaded; l != NULL; l = l->next) {
    RinRegistryEntry *e = (RinRegistryEntry *)l->data;
    const gchar *name = rin_registry_entry_get_name(e);
    if (g_strcmp0(name, "Soul Worker Plugin") == 0) {
      entry = e;
      break;
    }
  }

  if (!entry) {
    fprintf(stderr, "Soul Worker Plugin not found\n");
    rin_registry_unload(reg);
    g_object_unref(reg);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  // Set source to gf/datas
  char *src = g_build_filename(gf, "datas", NULL);

  // Check if src exists and is a directory
  if (!g_file_test(src, G_FILE_TEST_IS_DIR)) {
    fprintf(stderr, "Error: %s is not a valid directory\n", src);
    g_free(src);
    rin_registry_unload(reg);
    g_object_unref(reg);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  // Set destination
  char *dest;
  if (output_dir) {
    dest = g_strdup(output_dir);
  } else {
    dest = g_build_filename(gf, "extracted", NULL);
  }

  // Ensure dest directory exists
  if (g_mkdir_with_parents(dest, 0755) != 0) {
    fprintf(stderr, "Failed to create destination directory: %s\n", dest);
    g_free(src);
    g_free(dest);
    rin_registry_unload(reg);
    g_object_unref(reg);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  // Extract
  printf("Extracting archives from %s to %s...\n", src, dest);
  if (!rin_registry_entry_extract(entry, src, dest)) {
    fprintf(stderr, "Extraction failed\n");
    g_free(src);
    g_free(dest);
    rin_registry_unload(reg);
    g_object_unref(reg);
    g_strfreev(game_folder);
    g_free(output_dir);
    g_option_context_free(context);
    return 1;
  }

  printf("Extraction completed successfully.\n");

  g_free(src);
  g_free(dest);
  rin_registry_unload(reg);
  g_object_unref(reg);
  g_strfreev(game_folder);
  g_free(output_dir);
  g_option_context_free(context);
  return 0;
}
