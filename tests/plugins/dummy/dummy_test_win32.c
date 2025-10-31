#include <glib.h>
#include <gmodule.h>
#include <inttypes.h>
#include <stdio.h>

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
  rin_plugin_callbacks_t s_callbacks;
  void *p_instance;
} dummy_win32_fixture_t;

static
void
dummy_win32_report(void *opaque,
                   rin_plugin_report_t report,
                   char const *msg, float progress) {
  dummy_win32_fixture_t *fixture = opaque;

  switch (report) {
    case rin_plugin_report_debug: {
      g_print("[0x%" PRIxPTR ":debug] %s\n", opaque, msg);
    } break;
    case rin_plugin_report_info: {
      g_info("[0x%" PRIxPTR ":info] %s\n", opaque, msg);
    } break; 
    case rin_plugin_report_warning: {
      g_warning("[0x%" PRIxPTR ":warning] %s\n", opaque, msg);
    } break;
    case rin_plugin_report_error: {
      g_error("[0x%" PRIxPTR ":error] %s\n", opaque, msg);
    } break;
    case rin_plugin_report_progress: {
      g_print("[0x%" PRIxPTR ":progress] %.2f%% - %s\n", opaque, progress * 100.0f, msg);
    } break;
  }
}

static
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

static
void
dummy_win32_setup(dummy_win32_fixture_t *fixture,
                  gconstpointer user_data) {
  DUMMY_UNUSED(user_data);
  char const *plugin_path = user_data;
  fixture->s_callbacks.report = dummy_win32_report;
  fixture->s_callbacks.user_data = fixture;

  g_assert_nonnull(plugin_path);

  fixture->hModule = LoadLibrary(plugin_path);

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

  fixture->p_instance = fixture->p_interface->create(&fixture->s_callbacks);
  g_assert_nonnull(fixture->p_instance);

  fixture->p_interface->can_handle_dir(fixture->p_instance, "test_dir");
  fixture->p_interface->can_handle_file(fixture->p_instance, "test_file.rin");
  fixture->p_interface->extract(fixture->p_instance, "src", "dst");
  fixture->p_interface->build(fixture->p_instance, "src", "dst");
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
  g_assert_cmpint(fixture->p_info->i_capabilities & rin_plugin_caps_can_extract, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & rin_plugin_caps_can_build, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & rin_plugin_caps_can_handle_dir, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & rin_plugin_caps_supports_groups, !=, 0);
  g_assert_cmpint(fixture->p_info->i_capabilities & rin_plugin_caps_supports_validation, !=, 0);
}

int
main(int argc, char *argv[]) {
  g_test_init(&argc, &argv, NULL);

  g_test_add("/plugins/dummy/win32/interface",
    dummy_win32_fixture_t,
    argv[1],
    dummy_win32_setup,
    dummy_win32_get_interface,
    dummy_win32_teardown);

  g_test_add("/plugins/dummy/win32/info",
    dummy_win32_fixture_t,
    argv[1],
    dummy_win32_setup,
    dummy_win32_get_info,
    dummy_win32_teardown);

  g_test_add("/plugins/dummy/win32/info/fields",
    dummy_win32_fixture_t,
    argv[1],
    dummy_win32_setup,
    dummy_win32_check_info_fields,
    dummy_win32_teardown);

  return g_test_run();
}
