#include "rin-plugin-api.h"

#include <stdio.h>
#include <stdlib.h>

#define DUMMY_UNUSED(x) (void)(x)
#define DUMMY_TRUE (1)
#define DUMMY_FALSE (0)
#define DUMMY_LOG() fprintf(stdout, "Plugin call: %s\n", __func__)

struct dummy_context_s {
  int i_dummy_field;
  rin_plugin_callbacks_t *p_cbcs;
};

static
struct rin_plugin_info_s const *
dummy_get_info(void);

static
void *
dummy_create(rin_plugin_callbacks_t *p_cbcs) {
  struct dummy_context_s *context
    = calloc(1, sizeof(struct dummy_context_s));
  context->p_cbcs = p_cbcs;
  return context;
}

static
void
dummy_destroy(void *p_ctx) {
  DUMMY_LOG();
  struct dummy_context_s *ctx = p_ctx;
  free(ctx);
}

static
bool
dummy_can_handle_file(void *p_ctx, char const *psz_path) {
  struct dummy_context_s *ctx = p_ctx;
  DUMMY_LOG();
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy can_handle_file called", -0.0f);
  (void) p_ctx;
  (void) psz_path;
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy can't handle file", -0.0f);
  return false;
}

static
bool
dummy_can_handle_dir(void *p_ctx, char const *psz_path) {
  struct dummy_context_s *ctx = p_ctx;
  DUMMY_LOG();
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy can_handle_dir called", -0.0f);
  (void) p_ctx;
  (void) psz_path;
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy can't handle dir", -0.0f);
  return false;
}

static
rin_plugin_error_t
dummy_extract(void *p_ctx, char const *psz_src, char const *psz_dst) {
  struct dummy_context_s *ctx = p_ctx;
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy extract started", -0.0f);
  DUMMY_LOG();

  (void) p_ctx;
  (void) psz_src;
  (void) psz_dst;

  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy extract finished", -0.0f);
  return rin_plugin_error_internal;
}

static
rin_plugin_error_t
dummy_build(void *p_ctx, char const *psz_src, char const *psz_dst) {
  struct dummy_context_s *ctx = p_ctx;
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy build started", -0.0f);
  DUMMY_LOG();
  (void) p_ctx;
  (void) psz_src;
  (void) psz_dst;
  ctx
    ->p_cbcs
      ->report(ctx->p_cbcs->user_data,
               rin_plugin_report_info, "Dummy build finished", -0.0f);
  return rin_plugin_error_internal;
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
    = rin_plugin_caps_can_extract
        | rin_plugin_caps_can_build
        | rin_plugin_caps_can_handle_dir
        | rin_plugin_caps_supports_groups
        | rin_plugin_caps_supports_validation
};

struct rin_plugin_info_s const *
dummy_get_info(void) {
  return &dummy_plugin_info;
}

struct rin_plugin_interface_s const *
rin_plugin_get_interface(void) {
  return &dummy_plugin_interface;
}
