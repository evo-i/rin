#ifndef RIN_PLUGIN_API_H
#define RIN_PLUGIN_API_H 1

/**
 * @file rin-plugin-api.h
 * @brief Plugin API for the Rin archive extraction system
 *
 * This header defines the interface that plugins must implement to provide
 * archive extraction and building capabilities to Rin. The API uses versioning
 * to maintain compatibility between the core application and plugins.
 */

#include <stdint.h>
#include <stdbool.h>

/** @brief Major version number of the plugin API */
#define RIN_PLUGIN_API_VERSION_MAJOR 1
/** @brief Minor version number of the plugin API */
#define RIN_PLUGIN_API_VERSION_MINOR 0
/** @brief Patch version number of the plugin API */
#define RIN_PLUGIN_API_VERSION_PATCH 0

/** @brief Name of the function that plugins must export to provide their interface */
#define RIN_PLUGIN_GET_INTERFACE_FUNC_NAME "rin_plugin_get_interface"

/** @brief Helper macro for stringifying preprocessor tokens */
#define RIN_PLUGIN_API_STRINGIFY_HELPER(x) #x
/** @brief Macro for converting version numbers to string */
#define RIN_PLUGIN_API_STRINGIFY(x) RIN_PLUGIN_API_STRINGIFY_HELPER(x)

/** @brief Full version string of the plugin API in format "major.minor.patch" */
#define RIN_PLUGIN_API_CURRENT_VERSION_STRING \
  RIN_PLUGIN_API_STRINGIFY(RIN_PLUGIN_API_VERSION_MAJOR) "." \
  RIN_PLUGIN_API_STRINGIFY(RIN_PLUGIN_API_VERSION_MINOR) "." \
  RIN_PLUGIN_API_STRINGIFY(RIN_PLUGIN_API_VERSION_PATCH)

/**
 * @brief Encodes major, minor and patch version numbers into a single integer
 * 
 * @param major Major version number
 * @param minor Minor version number
 * @param patch Patch version number
 * @return Encoded version number
 */
#define RIN_PLUGIN_API_VERSION_ENCODE(major, minor, patch) \
    (((major) << 16) | ((minor) << 8) | (patch))

/** @brief Current API version encoded as a single integer */
#define RIN_PLUGIN_API_VERSION \
  RIN_PLUGIN_API_VERSION_ENCODE(RIN_PLUGIN_API_VERSION_MAJOR, \
                                RIN_PLUGIN_API_VERSION_MINOR, \
                                RIN_PLUGIN_API_VERSION_PATCH)

/** @brief Current API version for compatibility checks */
#define RIN_PLUGIN_API_CURRENT_VERSION RIN_PLUGIN_API_VERSION

/**
 * @brief Platform-specific calling convention for plugin functions
 * 
 * Ensures correct calling convention across different compilers:
 * - MSVC: Uses __cdecl
 * - GCC/Clang: Uses default calling convention
 */
#if defined(_MSC_VER)
  #define RIN_CALL __cdecl
#elif defined(__GNUC__) || defined(__clang__)
  #define RIN_CALL
#else
  #define RIN_CALL
#endif

/**
 * @brief Platform-specific symbol export/import macros
 * 
 * Controls symbol visibility for shared libraries:
 * - MSVC: Uses __declspec(dllexport/dllimport)
 * - GCC/Clang: Uses visibility("default")
 */
#if defined(_MSC_VER)
  #define RIN_EXPORT __declspec(dllexport)
  #define RIN_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
  #define RIN_EXPORT __attribute__((visibility("default")))
  #define RIN_IMPORT
#else
  #define RIN_EXPORT
  #define RIN_IMPORT
#endif

/**
 * @brief API declaration macro for plugin functions
 * 
 * Uses RIN_EXPORT when building a plugin, RIN_IMPORT when using a plugin
 */
#ifdef RIN_BUILD_PLUGIN
  #define RIN_API RIN_EXPORT
#else
  #define RIN_API RIN_IMPORT
#endif

/**
 * @brief Error codes returned by plugin operations
 */
typedef enum rin_plugin_error_e {
  /** @brief Operation completed successfully */
  rin_plugin_ok = 0,
  /** @brief Format is not supported by the plugin */
  rin_plugin_error_unsupported,
  /** @brief Input/output error occurred */
  rin_plugin_error_io,
  /** @brief File format is corrupted or invalid */
  rin_plugin_error_format,
  /** @brief Memory allocation failed */
  rin_plugin_error_memory,
  /** @brief Internal plugin error occurred */
  rin_plugin_error_internal,
  /** @brief Unknown error occurred */
  rin_plugin_error_unknown
} rin_plugin_error_t;

/**
 * @brief Types of messages that can be reported by plugins
 */
typedef enum rin_plugin_report_e {
  /** @brief Debug-level message */
  rin_plugin_report_debug,
  /** @brief Informational message */
  rin_plugin_report_info,
  /** @brief Warning message */
  rin_plugin_report_warning,
  /** @brief Error message */
  rin_plugin_report_error,
  /** @brief Progress update */
  rin_plugin_report_progress,
} rin_plugin_report_t;

/**
 * @brief Callback function type for plugin reporting
 *
 * @param opaque User data pointer passed to the plugin
 * @param report Type of report being made
 * @param msg Message text
 * @param progress Progress value (0.0 to 1.0) for progress reports
 */
typedef void
(RIN_CALL *rin_plugin_report_cb)
(void *opaque, rin_plugin_report_t report,
 char const *msg, float progress);

/**
 * @brief Structure containing callback functions provided to plugins
 */
typedef struct rin_plugin_callbacks_s {
  /** @brief Function for reporting messages and progress */
  rin_plugin_report_cb report;
  /** @brief User data pointer passed to callbacks */
  void *user_data;
} rin_plugin_callbacks_t;

/**
 * @brief Capability flags indicating plugin features
 */
typedef enum rin_plugin_caps_e {
  /** @brief No special capabilities */
  rin_plugin_caps_none                = 0,
  /** @brief Can extract files from archives */
  rin_plugin_caps_can_extract         = 1u << 0,
  /** @brief Can create new archives */
  rin_plugin_caps_can_build           = 1u << 1,
  /** @brief Supports file grouping within archives */
  rin_plugin_caps_supports_groups     = 1u << 2,
  /** @brief Supports context-aware operations */
  rin_plugin_caps_supports_context    = 1u << 3,
  /** @brief Supports limiting operations */
  rin_plugin_caps_supports_limit      = 1u << 4,
  /** @brief Supports text encoding conversion */
  rin_plugin_caps_supports_encoding   = 1u << 5,
  /** @brief Supports RBC format */
  rin_plugin_caps_supports_rbc        = 1u << 6,
  /** @brief Can read/write metadata */
  rin_plugin_caps_supports_metadata   = 1u << 7,
  /** @brief Can validate archive integrity */
  rin_plugin_caps_supports_validation = 1u << 8,
  /** @brief Can generate previews */
  rin_plugin_caps_supports_preview    = 1u << 9,
  /** @brief Requires external tools/libraries */
  rin_plugin_caps_requires_external   = 1u << 10,
  /** @brief Can process directories */
  rin_plugin_caps_can_handle_dir      = 1u << 11,
  /** @brief Maximum capability value */
  rin_plugin_caps_max                 = 0xffffffffu
} rin_plugin_caps_t;

/**
 * @brief Information about a plugin
 */
typedef struct rin_plugin_info_s {
  /** @brief Plugin name */
  const char *psz_name;
  /** @brief Plugin version string */
  const char *psz_version;
  /** @brief Plugin author name */
  const char *psz_author;
  /** @brief Plugin API interface version */
  uint32_t i_iface_version;
  /** @brief Bitmap of plugin capabilities */
  uint32_t i_capabilities;
} rin_plugin_info_t;

/**
 * @brief The main plugin interface structure
 *
 * This structure contains function pointers that define the plugin's interface.
 * Each plugin must implement all these functions to provide its functionality.
 */
typedef struct rin_plugin_interface_s {
  /**
   * @brief Get plugin information
   * @return Pointer to a static plugin info structure
   */
  rin_plugin_info_t const *
  (RIN_CALL *get_info)(void);

  /**
   * @brief Create a new plugin instance
   * @param cb Pointer to callback functions structure
   * @return Opaque pointer to plugin instance data
   */
  void *
  (RIN_CALL *create)(rin_plugin_callbacks_t *cb);

  /**
   * @brief Destroy a plugin instance
   * @param priv Plugin instance pointer from create()
   */
  void
  (RIN_CALL *destroy)(void *priv);

  /**
   * @brief Check if plugin can handle a file
   * @param priv Plugin instance pointer
   * @param file_name Path to the file to check
   * @return true if plugin can handle the file
   */
  bool
  (RIN_CALL *can_handle_file)(void *priv, char const *file_name);

  /**
   * @brief Check if plugin can handle a directory
   * @param priv Plugin instance pointer
   * @param dir_name Path to the directory to check
   * @return true if plugin can handle the directory
   */
  bool
  (RIN_CALL *can_handle_dir)(void *priv, char const *dir_name);

  /**
   * @brief Extract files from an archive
   * @param priv Plugin instance pointer
   * @param src Source archive path
   * @param dst Destination directory path
   * @return Error code indicating success or failure
   */
  rin_plugin_error_t
  (RIN_CALL *extract)(void *priv, char const *src, char const *dst);

  /**
   * @brief Create a new archive
   * @param priv Plugin instance pointer
   * @param src Source directory path
   * @param dst Destination archive path
   * @return Error code indicating success or failure
   */
  rin_plugin_error_t
  (RIN_CALL *build)(void *priv, char const *src, char const *dst);
} rin_plugin_interface_t;

#if defined(RIN_PLUGIN_BUILD)
/**
 * @brief Get the plugin interface
 *
 * This function must be exported by all plugins. It returns a pointer to
 * a static interface structure containing the plugin's implementation.
 *
 * @return Pointer to the plugin interface structure
 */
RIN_API
RIN_CALL
const rin_plugin_interface_t *
rin_plugin_get_interface(void);
#endif /* RIN_PLUGIN_BUILD */

#endif /* RIN_PLUGIN_API_H */
