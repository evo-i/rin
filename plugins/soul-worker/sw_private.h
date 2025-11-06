#ifndef SW_PRIVATE_H
#define SW_PRIVATE_H 1

#include "rin-plugin-api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct plugin_pair_s {
  char *key;
  char *value;
} plugin_pair_t;

typedef struct plugin_passwords_s {
  plugin_pair_t *pairs;
  size_t count;
} plugin_passwords_t;

typedef struct plugin_data_s {
  rin_plugin_callbacks_t *callbacks;
  plugin_passwords_t passwords;
} plugin_data_t;

typedef struct plugin_context_s {
  plugin_data_t data;
} plugin_context_t;

bool
plugin_get_passwords(plugin_context_t *ctx, char const *path, plugin_passwords_t *out_passwords);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SW_PRIVATE_H */
