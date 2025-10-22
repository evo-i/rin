#ifndef RIN_PLUGIN_API_H
#define RIN_PLUGIN_API_H 1

#include <stdint.h>

#define RIN_PLUGIN_API_VERSION_MAJOR 1
#define RIN_PLUGIN_API_VERSION_MINOR 0
#define RIN_PLUGIN_API_VERSION_PATCH 0

#define RIN_PLUGIN_GET_INTERFACE_FUNC_NAME "rin_plugin_get_interface"

#define RIN_PLUGIN_API_STRINGIFY_HELPER(x) #x
#define RIN_PLUGIN_API_STRINGIFY(x) RIN_PLUGIN_API_STRINGIFY_HELPER(x)

#define RIN_PLUGIN_API_CURRENT_VERSION_STRING \
    RIN_PLUGIN_API_STRINGIFY(RIN_PLUGIN_API_VERSION_MAJOR) "." \
    RIN_PLUGIN_API_STRINGIFY(RIN_PLUGIN_API_VERSION_MINOR) "." \
    RIN_PLUGIN_API_STRINGIFY(RIN_PLUGIN_API_VERSION_PATCH)

#define RIN_PLUGIN_API_VERSION_ENCODE(major, minor, patch) \
    (((major) << 16) | ((minor) << 8) | (patch))

#define RIN_PLUGIN_API_VERSION \
    RIN_PLUGIN_API_VERSION_ENCODE(RIN_PLUGIN_API_VERSION_MAJOR, \
                                  RIN_PLUGIN_API_VERSION_MINOR, \
                                  RIN_PLUGIN_API_VERSION_PATCH)

#define RIN_PLUGIN_API_CURRENT_VERSION RIN_PLUGIN_API_VERSION

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

#ifdef RIN_BUILD_PLUGIN
  #define RIN_API RIN_EXPORT
#else
  #define RIN_API RIN_IMPORT
#endif

typedef enum rin_plugin_caps_e {
  RIN_PLUGIN_CAPS_NONE                = 0,
  RIN_PLUGIN_CAPS_CAN_EXTRACT         = 1u << 0,
  RIN_PLUGIN_CAPS_CAN_BUILD           = 1u << 1,
  RIN_PLUGIN_CAPS_SUPPORTS_GROUPS     = 1u << 2,
  RIN_PLUGIN_CAPS_SUPPORTS_CONTEXT    = 1u << 3,
  RIN_PLUGIN_CAPS_SUPPORTS_LIMIT      = 1u << 4,
  RIN_PLUGIN_CAPS_SUPPORTS_ENCODING   = 1u << 5,
  RIN_PLUGIN_CAPS_SUPPORTS_RBC        = 1u << 6,
  RIN_PLUGIN_CAPS_SUPPORTS_METADATA   = 1u << 7,
  RIN_PLUGIN_CAPS_SUPPORTS_VALIDATION = 1u << 8,
  RIN_PLUGIN_CAPS_SUPPORTS_PREVIEW    = 1u << 9,
  RIN_PLUGIN_CAPS_REQUIRES_EXTERNAL   = 1u << 10,
  RIN_PLUGIN_CAPS_CAN_HANDLE_DIR      = 1u << 11,
  RIN_PLUGIN_CAPS_MAX                 = 0xffffffffu
} rin_plugin_caps_t;

typedef struct rin_plugin_info_s {
  const char *psz_name;
  const char *psz_version;
  const char *psz_author;
  uint32_t i_iface_version;
  uint32_t i_capabilities;
} rin_plugin_info_t;

typedef struct rin_plugin_interface_s {
  const rin_plugin_info_t *
  (*get_info)(void);

  void *
  (*create)(void);

  void
  (*destroy)(void *priv);

  int
  (*can_handle_file)(void *priv, const char *file_name);

  int
  (*can_handle_dir)(void *priv, const char *dir_name);

  int
  (*extract)(void *priv, const char *src, const char *dst);

  int
  (*build)(void *priv, const char *src, const char *dst);
} rin_plugin_interface_t;

#if defined(RIN_PLUGIN_BUILD)
RIN_API
const rin_plugin_interface_t *
rin_plugin_get_interface(void);
#endif /* RIN_PLUGIN_BUILD */

#endif /* RIN_PLUGIN_API_H */
