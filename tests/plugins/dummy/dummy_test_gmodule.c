#include <glib.h>
#include <gmodule.h>

#include "rin-plugin-api.h"

#define DUMMY_UNUSED(x) (void)(x)
#define DUMMY_NAME "Dummy Plugin"
#define DUMMY_AUTHOR "Rin Development Team"
#define DUMMY_VERSION RIN_PLUGIN_API_CURRENT_VERSION_STRING

typedef struct dummy_gmodule_fixture_s {
  GModule *module;
  rin_plugin_interface_t const *p_interface;
  rin_plugin_info_t const *p_info;
  void *p_instance;
} dummy_gmodule_fixture_t;

void
dummy_gmodule_teardown(dummy_gmodule_fixture_t *fixture,
                       gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  if (fixture->p_interface) {
    fixture->p_interface->destroy(fixture->p_instance);
  }
  if (fixture->module) {
    g_module_close(fixture->module);
  }
}

void
dummy_gmodule_setup(dummy_gmodule_fixture_t *fixture,
                    gconstpointer user_data) {
  DUMMY_UNUSED(user_data);

  /* Prefer paths from RIN_PLUGIN_PATH (search multiple dirs separated by
   * G_SEARCHPATH_SEPARATOR), fall back to a relative build path. */
  char const *plugin_path = user_data;
  g_assert_nonnull(plugin_path);

  fixture->module = g_module_open(plugin_path, G_MODULE_BIND_LOCAL);
  g_assert_nonnull(fixture->module);

  typedef rin_plugin_interface_t const *(*get_interface_func_t)(void);
  get_interface_func_t get_interface = NULL;
  gboolean have_sym
    = g_module_symbol(fixture->module,
                      RIN_PLUGIN_GET_INTERFACE_FUNC_NAME,
                      (gpointer *) &get_interface);
  g_assert_true(have_sym);
  g_assert_nonnull(get_interface);

  fixture->p_interface = get_interface();
  g_assert_nonnull(fixture->p_interface);

  fixture->p_info = fixture->p_interface->get_info();
  g_assert_nonnull(fixture->p_info);

  fixture->p_instance = fixture->p_interface->create();
  g_assert_nonnull(fixture->p_instance);
}

void
dummy_gmodule_get_interface(dummy_gmodule_fixture_t *fixture,
                            gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  typedef rin_plugin_interface_t const *(*get_interface_func_t)(void);
  get_interface_func_t get_interface = NULL;
  gboolean have_sym
    = g_module_symbol(fixture->module,
                      RIN_PLUGIN_GET_INTERFACE_FUNC_NAME,
                      (gpointer *)&get_interface);
  g_assert_true(have_sym);
  g_assert_nonnull(get_interface);

  rin_plugin_interface_t const *iface = get_interface();
  g_assert_nonnull(iface);
}

void
dummy_gmodule_get_info(dummy_gmodule_fixture_t *fixture,
                       gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  rin_plugin_info_t const *info = fixture->p_interface->get_info();
  g_assert_nonnull(info);
}

void
dummy_gmodule_check_info_fields(dummy_gmodule_fixture_t *fixture,
                                gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  g_assert_nonnull(fixture->p_info->psz_name);
  g_assert_nonnull(fixture->p_info->psz_version);
  g_assert_nonnull(fixture->p_info->psz_author);
  g_assert_cmpuint(fixture->p_info->i_iface_version,
                   ==, RIN_PLUGIN_API_CURRENT_VERSION);

  g_assert_cmpstr(fixture->p_info->psz_author, ==, DUMMY_AUTHOR);
  g_assert_cmpstr(fixture->p_info->psz_version, ==, DUMMY_VERSION);
  g_assert_cmpstr(fixture->p_info->psz_name, ==, DUMMY_NAME);
  g_assert_cmpint(fixture->p_info->i_capabilities & RIN_PLUGIN_CAPS_CAN_EXTRACT, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & RIN_PLUGIN_CAPS_CAN_BUILD, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & RIN_PLUGIN_CAPS_CAN_HANDLE_DIR, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & RIN_PLUGIN_CAPS_SUPPORTS_GROUPS, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & RIN_PLUGIN_CAPS_SUPPORTS_VALIDATION, !=, 0);
}

int
main(int argc, char *argv[]) {
  g_test_init(&argc, &argv, NULL);

  g_test_add("/plugins/dummy/gmodule/interface",
    dummy_gmodule_fixture_t,
    argv[1],
    dummy_gmodule_setup,
    dummy_gmodule_get_interface,
    dummy_gmodule_teardown);

  g_test_add("/plugins/dummy/gmodule/info",
    dummy_gmodule_fixture_t,
    argv[1],
    dummy_gmodule_setup,
    dummy_gmodule_get_info,
    dummy_gmodule_teardown);

  g_test_add("/plugins/dummy/gmodule/info/fields",
    dummy_gmodule_fixture_t,
    argv[1],
    dummy_gmodule_setup,
    dummy_gmodule_check_info_fields,
    dummy_gmodule_teardown);

  return g_test_run();
}
