#include "sw_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
test_report_callback(void* user_data, rin_plugin_report_t report, const char* msg, float progress) {
  const char* type;
  switch (report) {
  case rin_plugin_report_debug:
    type = "DEBUG";
    break;
  case rin_plugin_report_info:
    type = "INFO";
    break;
  case rin_plugin_report_warning:
    type = "WARNING";
    break;
  case rin_plugin_report_error:
    type = "ERROR";
    break;
  case rin_plugin_report_progress:
    type = "PROGRESS";
    break;
  default:
    type = "UNKNOWN";
    break;
  }

  if (report == rin_plugin_report_progress) {
    printf("[%s] %s (%.1f%%)\n", type, msg, progress * 100.0f);
  } else {
    printf("[%s] %s\n", type, msg);
  }
}

int
main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path_to_soul_worker_directory>\n", argv[0]);
    fprintf(stderr, "Example: %s \"C:\\Games\\SoulWorker\"\n", argv[0]);
    return 1;
  }

  const char* sw_path = argv[1];

  // Setup callbacks
  rin_plugin_callbacks_t callbacks;
  callbacks.report    = test_report_callback;
  callbacks.user_data = NULL;

  // Output initial message via callback
  char msg_buf[512];
  snprintf(msg_buf, sizeof(msg_buf), "Testing Soul Worker plugin with path: %s", sw_path);
  callbacks.report(callbacks.user_data, rin_plugin_report_info, msg_buf, 0.0f);

  // Create context
  plugin_context_t ctx;
  ctx.data.callbacks = &callbacks;

  // Initialize passwords structure
  plugin_passwords_t passwords;
  passwords.pairs = NULL;
  passwords.count = 0;

  // Call the function
  callbacks.report(callbacks.user_data, rin_plugin_report_info,
                   "Extracting passwords from SoulWorker64.dll...", 0.0f);
  bool result = plugin_get_passwords(&ctx, sw_path, &passwords);

  if (!result) {
    callbacks.report(callbacks.user_data, rin_plugin_report_error, "Failed to extract passwords!",
                     0.0f);
    return 1;
  }

  // Output passwords via callback
  snprintf(msg_buf, sizeof(msg_buf), "Successfully extracted %zu passwords", passwords.count);
  callbacks.report(callbacks.user_data, rin_plugin_report_info, msg_buf, 0.0f);

  for (size_t i = 0; i < passwords.count; i++) {
    snprintf(msg_buf, sizeof(msg_buf), "%s: %s", passwords.pairs[i].key, passwords.pairs[i].value);
    callbacks.report(callbacks.user_data, rin_plugin_report_info, msg_buf, 0.0f);
  }

  // Cleanup
  for (size_t i = 0; i < passwords.count; i++) {
    free(passwords.pairs[i].key);
    free(passwords.pairs[i].value);
  }
  free(passwords.pairs);

  callbacks.report(callbacks.user_data, rin_plugin_report_info, "Test completed successfully!",
                   0.0f);
  return 0;
}
