#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <stdbool.h>

/**
 * @brief Opaque plugin handle
 */
typedef struct plugin_s plugin_t;

/**
 * @brief Create a new plugin loader instance
 * @return Plugin instance or NULL on failure
 */
plugin_t*
plugin_new(void);

/**
 * @brief Load a plugin from a shared library file
 * @param plugin Plugin instance
 * @param path Path to the plugin library (.dll or .so)
 * @return true on success, false on failure
 */
bool
plugin_load(plugin_t* plugin, const char* path);

/**
 * @brief Get the plugin name
 * @param plugin Plugin instance
 * @return Plugin name or NULL if not loaded
 */
const char*
plugin_get_name(plugin_t* plugin);

/**
 * @brief Check if plugin can handle a directory
 * @param plugin Plugin instance
 * @param dir_path Directory path to check
 * @return true if plugin can handle the directory
 */
bool
plugin_can_handle_dir(plugin_t* plugin, const char* dir_path);

/**
 * @brief Check if plugin can handle a file
 * @param plugin Plugin instance
 * @param file_path File path to check
 * @return true if plugin can handle the file
 */
bool
plugin_can_handle_file(plugin_t* plugin, const char* file_path);

/**
 * @brief Extract files using the plugin
 * @param plugin Plugin instance
 * @param src Source path (file or directory)
 * @param dst Destination directory
 * @return true on success, false on failure
 */
bool
plugin_extract(plugin_t* plugin, const char* src, const char* dst);

/**
 * @brief Free plugin resources
 * @param plugin Plugin instance to free
 */
void
plugin_free(plugin_t* plugin);

#endif /* PLUGIN_LOADER_H */
