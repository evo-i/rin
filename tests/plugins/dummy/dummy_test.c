#include <glib.h>
#include <gmodule.h>

#include <windows.h>

#include "rin-plugin-api.h"

#define DUMMY_UNUSED(x) (void)(x)
#define DUMMY_NAME "Dummy Plugin"
#define DUMMY_AUTHOR "Rin Development Team"
#define DUMMY_VERSION RIN_PLUGIN_API_CURRENT_VERSION_STRING

typedef struct dummy_win32_fixture_s {
  HMODULE hModule;
  DLL_DIRECTORY_COOKIE dll_cookie;
  rin_plugin_interface_t const *p_interface;
  rin_plugin_info_t const *p_info;
  void *p_instance;
} dummy_win32_fixture_t;

void
dummy_win32_teardown(dummy_win32_fixture_t *fixture,
                     gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  if (fixture->p_interface) {
    fixture->p_interface->destroy(fixture->p_instance);
  }

  if (fixture->hModule) {
    FreeLibrary(fixture->hModule);
  }
}

void
dummy_win32_setup(dummy_win32_fixture_t *fixture,
                  gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
   g_getenv("RIN_PLUGIN_PATH");
  char path_buffer[MAX_PATH];
  GetModuleFileNameA(NULL, path_buffer, MAX_PATH);
  fixture->hModule
    = LoadLibrary("F:\\msys64\\home\\devbi\\rin\\build\\plugins\\dummy\\libdummy_plugin.dll");
  g_assert_nonnull(fixture->hModule);

  typedef rin_plugin_interface_t const *(*get_interface_func_t)(void);
  get_interface_func_t get_interface
    = (get_interface_func_t) GetProcAddress(fixture->hModule,
                                            RIN_PLUGIN_GET_INTERFACE_FUNC_NAME);
  g_assert_nonnull(get_interface);

  fixture->p_interface = get_interface();
  g_assert_nonnull(fixture->p_interface);

  fixture->p_info = fixture->p_interface->get_info();
  g_assert_nonnull(fixture->p_info);

  fixture->p_instance = fixture->p_interface->create();
  g_assert_nonnull(fixture->p_instance);
}

void
dummy_win32_get_interface(dummy_win32_fixture_t *fixture,
                          gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  typedef rin_plugin_interface_t const *(*get_interface_func_t)(void);
  get_interface_func_t get_interface
    = (get_interface_func_t) GetProcAddress(fixture->hModule,
                                            RIN_PLUGIN_GET_INTERFACE_FUNC_NAME);
  g_assert_nonnull(get_interface);

  rin_plugin_interface_t const *iface = get_interface();
  g_assert_nonnull(iface);
}

void
dummy_win32_get_info(dummy_win32_fixture_t *fixture,
                      gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  rin_plugin_info_t const *info = fixture->p_interface->get_info();
  g_assert_nonnull(info);
}

void
dummy_win32_check_info_fields(dummy_win32_fixture_t *fixture,
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

  g_test_add("/plugins/dummy/win32/interface",
    dummy_win32_fixture_t,
    NULL,
    dummy_win32_setup,
    dummy_win32_get_interface,
    dummy_win32_teardown);

  g_test_add("/plugins/dummy/win32/info",
    dummy_win32_fixture_t,
    NULL,
    dummy_win32_setup,
    dummy_win32_get_info,
    dummy_win32_teardown);

  g_test_add("/plugins/dummy/win32/info/fields",
    dummy_win32_fixture_t,
    NULL,
    dummy_win32_setup,
    dummy_win32_check_info_fields,
    dummy_win32_teardown);

  return g_test_run();
}
