#include "plugin_loader.h"
#include "../../src/rin-plugin-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define PLUGIN_HANDLE HMODULE
#define PLUGIN_SYM(handle, name) GetProcAddress(handle, name)
#define PLUGIN_UNLOAD(handle) FreeLibrary(handle)
#define PLUGIN_ERROR() "LoadLibrary failed"
#else
#include <dlfcn.h>
#define PLUGIN_HANDLE void*
#define PLUGIN_SYM(handle, name) dlsym(handle, name)
#define PLUGIN_UNLOAD(handle) dlclose(handle)
#define PLUGIN_ERROR() dlerror()
#endif

// Helper to extract directory from path
static char*
get_directory(const char* path) {
  if (!path)
    return NULL;

  const char* last_sep = strrchr(path, '/');
#ifdef _WIN32
  const char* last_back = strrchr(path, '\\');
  if (last_back && (!last_sep || last_back > last_sep)) {
    last_sep = last_back;
  }
#endif

  if (!last_sep)
    return NULL;

  size_t len = last_sep - path;
  char*  dir = malloc(len + 1);
  if (!dir)
    return NULL;

  memcpy(dir, path, len);
  dir[len] = '\0';
  return dir;
}

// Load plugin with deps directory support
static PLUGIN_HANDLE
load_plugin_with_deps(const char* path) {
  char* plugin_dir = get_directory(path);
  if (!plugin_dir) {
    return NULL;
  }

  // Build path to deps directory
  size_t deps_path_len = strlen(plugin_dir) + 10; // "/deps" + null
  char*  deps_path     = malloc(deps_path_len);
  if (!deps_path) {
    free(plugin_dir);
    return NULL;
  }

#ifdef _WIN32
  snprintf(deps_path, deps_path_len, "%s\\deps", plugin_dir);

  // Convert to wide char for AddDllDirectory
  wchar_t wdeps_path[MAX_PATH];
  MultiByteToWideChar(CP_UTF8, 0, deps_path, -1, wdeps_path, MAX_PATH);

  // Add deps directory to DLL search path
  HMODULE              handle = NULL;
  DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(wdeps_path);
  if (cookie) {
    // Load with altered search path
    handle = LoadLibraryExA(path, NULL,
                            LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
    // Don't remove the cookie - keep it for the lifetime of the plugin
  } else {
    // Fallback to regular load
    handle = LoadLibraryA(path);
  }

  free(deps_path);
  free(plugin_dir);
  return handle;
#else
  snprintf(deps_path, deps_path_len, "%s/deps", plugin_dir);

  // On Linux, add deps directory to LD_LIBRARY_PATH temporarily
  char* old_ld_path = getenv("LD_LIBRARY_PATH");
  char* new_ld_path = NULL;

  if (old_ld_path) {
    size_t new_len = strlen(old_ld_path) + strlen(deps_path) + 2;
    new_ld_path    = malloc(new_len);
    if (new_ld_path) {
      snprintf(new_ld_path, new_len, "%s:%s", deps_path, old_ld_path);
      setenv("LD_LIBRARY_PATH", new_ld_path, 1);
    }
  } else {
    setenv("LD_LIBRARY_PATH", deps_path, 1);
  }

  PLUGIN_HANDLE handle = dlopen(path, RTLD_LAZY);

  // Restore original LD_LIBRARY_PATH
  if (old_ld_path) {
    setenv("LD_LIBRARY_PATH", old_ld_path, 1);
  } else {
    unsetenv("LD_LIBRARY_PATH");
  }

  free(new_ld_path);
  free(deps_path);
  free(plugin_dir);
  return handle;
#endif
}

struct plugin_s {
  PLUGIN_HANDLE                 handle;
  const rin_plugin_interface_t* iface;
  void*                         priv;
  rin_plugin_callbacks_t        callbacks;
};

static void
default_report_cb(void* opaque, rin_plugin_report_t report, const char* msg, float progress) {
  (void)opaque;

  switch (report) {
  case rin_plugin_report_debug:
    fprintf(stderr, "[DEBUG] %s\n", msg);
    break;
  case rin_plugin_report_info:
    fprintf(stdout, "[INFO] %s\n", msg);
    break;
  case rin_plugin_report_warning:
    fprintf(stderr, "[WARNING] %s\n", msg);
    break;
  case rin_plugin_report_error:
    fprintf(stderr, "[ERROR] %s\n", msg);
    break;
  case rin_plugin_report_progress:
    fprintf(stdout, "[PROGRESS] %s (%.1f%%)\n", msg, progress * 100.0f);
    break;
  }
}

plugin_t*
plugin_new(void) {
  plugin_t* plugin = calloc(1, sizeof(plugin_t));
  if (!plugin) {
    return NULL;
  }

  plugin->callbacks.report    = default_report_cb;
  plugin->callbacks.user_data = NULL;

  return plugin;
}

bool
plugin_load(plugin_t* plugin, const char* path) {
  if (!plugin || !path) {
    return false;
  }

  // Load the shared library with deps directory support
  plugin->handle = load_plugin_with_deps(path);
  if (!plugin->handle) {
    fprintf(stderr, "Failed to load plugin %s: %s\n", path, PLUGIN_ERROR());
    return false;
  }

  // Get the interface function
  typedef const rin_plugin_interface_t* (*get_iface_fn)(void);
  get_iface_fn get_iface =
      (get_iface_fn)PLUGIN_SYM(plugin->handle, RIN_PLUGIN_GET_INTERFACE_FUNC_NAME);
  if (!get_iface) {
    fprintf(stderr, "Failed to find %s in %s\n", RIN_PLUGIN_GET_INTERFACE_FUNC_NAME, path);
    PLUGIN_UNLOAD(plugin->handle);
    plugin->handle = NULL;
    return false;
  }

  // Get the interface
  plugin->iface = get_iface();
  if (!plugin->iface) {
    fprintf(stderr, "Failed to get plugin interface from %s\n", path);
    PLUGIN_UNLOAD(plugin->handle);
    plugin->handle = NULL;
    return false;
  }

  // Verify API version compatibility
  const rin_plugin_info_t* info = plugin->iface->get_info();
  if (!info) {
    fprintf(stderr, "Failed to get plugin info from %s\n", path);
    PLUGIN_UNLOAD(plugin->handle);
    plugin->handle = NULL;
    plugin->iface  = NULL;
    return false;
  }

  if (info->i_iface_version != RIN_PLUGIN_API_CURRENT_VERSION) {
    fprintf(stderr, "Plugin API version mismatch: plugin=%u, expected=%u\n", info->i_iface_version,
            RIN_PLUGIN_API_CURRENT_VERSION);
    PLUGIN_UNLOAD(plugin->handle);
    plugin->handle = NULL;
    plugin->iface  = NULL;
    return false;
  }

  // Create plugin instance
  plugin->priv = plugin->iface->create(&plugin->callbacks);
  if (!plugin->priv) {
    fprintf(stderr, "Failed to create plugin instance from %s\n", path);
    PLUGIN_UNLOAD(plugin->handle);
    plugin->handle = NULL;
    plugin->iface  = NULL;
    return false;
  }

  fprintf(stdout, "Loaded plugin: %s (%s)\n", info->psz_name, info->psz_version);

  return true;
}

const char*
plugin_get_name(plugin_t* plugin) {
  if (!plugin || !plugin->iface) {
    return NULL;
  }

  const rin_plugin_info_t* info = plugin->iface->get_info();
  return info ? info->psz_name : NULL;
}

bool
plugin_can_handle_dir(plugin_t* plugin, const char* dir_path) {
  if (!plugin || !plugin->iface || !plugin->priv || !dir_path) {
    return false;
  }

  if (!plugin->iface->can_handle_dir) {
    return false;
  }

  return plugin->iface->can_handle_dir(plugin->priv, dir_path);
}

bool
plugin_can_handle_file(plugin_t* plugin, const char* file_path) {
  if (!plugin || !plugin->iface || !plugin->priv || !file_path) {
    return false;
  }

  if (!plugin->iface->can_handle_file) {
    return false;
  }

  return plugin->iface->can_handle_file(plugin->priv, file_path);
}

bool
plugin_extract(plugin_t* plugin, const char* src, const char* dst) {
  if (!plugin || !plugin->iface || !plugin->priv || !src || !dst) {
    return false;
  }

  if (!plugin->iface->extract) {
    fprintf(stderr, "Plugin does not support extraction\n");
    return false;
  }

  rin_plugin_error_t err = plugin->iface->extract(plugin->priv, src, dst);
  if (err != rin_plugin_ok) {
    fprintf(stderr, "Extraction failed with error code: %d\n", err);
    return false;
  }

  return true;
}

void
plugin_free(plugin_t* plugin) {
  if (!plugin) {
    return;
  }

  if (plugin->priv && plugin->iface && plugin->iface->destroy) {
    plugin->iface->destroy(plugin->priv);
  }

  if (plugin->handle) {
    PLUGIN_UNLOAD(plugin->handle);
  }

  free(plugin);
}
