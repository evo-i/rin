#include "rin-plugin-api.h"

#include <stdio.h>
#include <stdlib.h>

#define DUMMY_UNUSED(x) (void)(x)
#define DUMMY_TRUE (1)
#define DUMMY_FALSE (0)
#define DUMMY_LOG() fprintf(stderr, "Plugin call: %s\n", __func__)

struct dummy_context_s {
  int dummy_field;
};

static
struct rin_plugin_info_s const *
dummy_get_info(void);

static
void *
dummy_create(void) {
  return calloc(1, sizeof(struct dummy_context_s));
}

static
void
dummy_destroy(void *p_ctx) {
  DUMMY_LOG();
  struct dummy_context_s *ctx = p_ctx;
  free(ctx);
}

static
int
dummy_can_handle_file(void *p_ctx, char const *psz_path) {
  DUMMY_LOG();
  (void) p_ctx;
  (void) psz_path;
  return DUMMY_FALSE;
}

static
int
dummy_can_handle_dir(void *p_ctx, char const *psz_path) {
  DUMMY_LOG();
  (void) p_ctx;
  (void) psz_path;
  return DUMMY_FALSE;
}

static
int
dummy_extract(void *p_ctx, char const *psz_src, char const *psz_dst) {
  DUMMY_LOG();
  (void) p_ctx;
  (void) psz_src;
  (void) psz_dst;
  return DUMMY_FALSE;
}

static
int
dummy_build(void *p_ctx, char const *psz_src, char const *psz_dst) {
  DUMMY_LOG();
  (void) p_ctx;
  (void) psz_src;
  (void) psz_dst;
  return DUMMY_FALSE;
}

static struct rin_plugin_interface_s const
dummy_plugin_interface = {
  .get_info = dummy_get_info,
  .create = dummy_create,
  .destroy = dummy_destroy,
  .can_handle_file = dummy_can_handle_file,
  .can_handle_dir = dummy_can_handle_dir,
  .extract = dummy_extract,
  .build = dummy_build
};

static struct rin_plugin_info_s const
dummy_plugin_info = {
  .psz_name = "Dummy Plugin",
  .psz_version = RIN_PLUGIN_API_CURRENT_VERSION_STRING,
  .psz_author = "Rin Development Team",
  .i_iface_version = RIN_PLUGIN_API_CURRENT_VERSION,
  .i_capabilities
    = RIN_PLUGIN_CAPS_CAN_EXTRACT
        | RIN_PLUGIN_CAPS_CAN_BUILD
        | RIN_PLUGIN_CAPS_CAN_HANDLE_DIR
        | RIN_PLUGIN_CAPS_SUPPORTS_GROUPS
        | RIN_PLUGIN_CAPS_SUPPORTS_VALIDATION
};

struct rin_plugin_info_s const *
dummy_get_info(void) {
  return &dummy_plugin_info;
}

struct rin_plugin_interface_s const *
rin_plugin_get_interface(void) {
  return &dummy_plugin_interface;
}
