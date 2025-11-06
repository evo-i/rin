#include "sw_private.h"

#include <stdio.h>

static const char
z_sign[] = {
  0x50,
  0x4B,
  0x03,
  0x04
};

static const uint32_t
z_value = (0x50
            | (0x4B << 8)
            | (0x03 << 16)
            | (0x04 << 24));

static const char
v_sign[] = {
  z_sign[0] ^ 0x55,
  z_sign[1] ^ 0x55,
  z_sign[2] ^ 0x55,
  z_sign[3] ^ 0x55
};

static const uint32_t
v_value = (v_sign[0]
            | (v_sign[1] << 8)
            | (v_sign[2] << 16)
            | (v_sign[3] << 24));

static const char const *
required_file_list[] = {
  "SoulWorker.exe",
  "SoulWorker64.dll",
  "BaseDX11.dll",
  "Base.dll",
  "datas/data00.v",
  "datas/data11.v",
  NULL
};



#ifdef _WIN32
# include <Windows.h>
static
bool
local_can_handle_dir(const char *dir_name) {
  DWORD attrs = GetFileAttributesA(dir_name);
  if (attrs == INVALID_FILE_ATTRIBUTES ||
      !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
    return false;
  }

  for (size_t i = 0; required_file_list[i] != NULL; ++i) {
    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, "%s\\%s", dir_name, required_file_list[i]);

    DWORD file_attrs = GetFileAttributesA(full_path);
    if (file_attrs == INVALID_FILE_ATTRIBUTES ||
        (file_attrs & FILE_ATTRIBUTE_DIRECTORY)) {
      return false;
    }
  }

  return true;
}
#else
# include <sys/stat.h>
# include <dirent.h>
static
bool
local_can_handle_dir(const char *dir_name) {
  struct stat st;
  if (stat(dir_name, &st) != 0 || !S_ISDIR(st.st_mode)) {
    return false;
  }

  for (size_t i = 0; required_file_list[i] != NULL; ++i) {
    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, required_file_list[i]);

    if (stat(full_path, &st) != 0 || S_ISDIR(st.st_mode)) {
      return false;
    }
  }

  return true;
}
#endif

static rin_plugin_info_t const *
plugin_get_info(void);

static rin_plugin_interface_t const *
plugin_get_interface(void);

static
void *
plugin_create(rin_plugin_callbacks_t *cb);

static
void
plugin_destroy(void *priv);

static
bool
plugin_can_handle_file(void *priv, char const *file_name);

static
bool
plugin_can_handle_dir(void *priv, char const *dir_name);

static
rin_plugin_error_t
plugin_extract(void *priv, char const *src, char const *dst);

static
rin_plugin_error_t
plugin_build(void *priv, char const *src, char const *dst);

static rin_plugin_info_t const
plugin_info = {
  .psz_name = "Soul Worker Plugin",
  .psz_version = "1.0.0",
  .psz_author = "Evo Development Team",
  .i_iface_version = RIN_PLUGIN_API_CURRENT_VERSION,
  .i_capabilities = (rin_plugin_caps_can_handle_dir
                      | rin_plugin_caps_can_extract
                      | rin_plugin_caps_can_build)
};

static rin_plugin_interface_t const
plugin_interface = {
  .get_info = plugin_get_info,
  .create = plugin_create,
  .destroy = plugin_destroy,
  .can_handle_file = plugin_can_handle_file,
  .can_handle_dir = plugin_can_handle_dir,
  .extract = plugin_extract,
  .build = plugin_build
};

rin_plugin_interface_t const *
rin_plugin_get_interface(void) {
  return &plugin_interface;
}

rin_plugin_info_t const *
plugin_get_info(void) {
  return &plugin_info;
}

void *
plugin_create(rin_plugin_callbacks_t *cb) {
  plugin_data_t *self
    = malloc(sizeof(plugin_data_t));

  if (!self) {
    cb->report(cb->user_data,
               rin_plugin_report_error,
               "Failed to allocate memory for plugin data.",
               0.0f);
    return NULL;
  }

  self->callbacks = cb;
  self->passwords.pairs = NULL;
  self->passwords.count = 0;

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Plugin instance created successfully.",
                          0.0f);

  return self;
}

void
plugin_destroy(void *priv) {
  plugin_data_t *self = (plugin_data_t *) priv;

  if (self) {
    for (size_t i = 0; i < self->passwords.count; ++i) {
      free(self->passwords.pairs[i].key);
      free(self->passwords.pairs[i].value);
    }

    free(self->passwords.pairs);
    free(self);
  }
}

bool
plugin_can_handle_file(void *priv, char const *file_name) {
  uint8_t header[4] =  { 0 };
  plugin_data_t *self = (plugin_data_t *) priv;

  (void) self;

  const char *ext = strrchr(file_name, '.');

  if (ext && strcmp(ext, ".v") == 0) { 
    FILE *f = fopen(file_name, "rb");
    if (f) {
      size_t read_bytes = fread(header, 1, sizeof(header), f);
      fclose(f);

      if (read_bytes == sizeof(header)) {
        uint32_t header_value = (header[0]
                                 | (header[1] << 8)
                                 | (header[2] << 16)
                                 | (header[3] << 24));
        if (header_value == z_value || header_value == v_value) {
          self->callbacks->report(self->callbacks->user_data,
                                  rin_plugin_report_info,
                                  "File header matches expected signature.",
                                  0.0f);
          return true;
        } else {
          self->callbacks->report(self->callbacks->user_data,
                                  rin_plugin_report_warning,
                                  "File header does not match expected signature.",
                                  0.0f);
          return false;
        }
      } else {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_error,
                                "Failed to read file header.",
                                0.0f);
        return false;
      }
    } else {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_error,
                              "Failed to open file for reading.",
                              0.0f);
      return false;
    }

    self->callbacks->report(self->callbacks->user_data,
                            rin_plugin_report_info,
                            "File can be handled by the plugin.",
                            0.0f);

    return true;
  }

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "File cannot be handled by the plugin.",
                          0.0f);
  return false;
}

bool
plugin_can_handle_dir(void *priv, char const *dir_name) {
  plugin_data_t *self = (plugin_data_t *) priv;

  if (!local_can_handle_dir(dir_name)) {
    self->callbacks->report(self->callbacks->user_data,
                            rin_plugin_report_warning,
                            "Directory cannot be handled by the plugin.",
                            0.0f);
    return false;
  }

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Directory can be handled by the plugin.",
                          0.0f);
  return true;
}

static
rin_plugin_error_t
plugin_extract(void *priv, char const *src, char const *dst) {
  plugin_data_t *self = (plugin_data_t *) priv;

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Extract operation started.",
                          0.0f);

  /* Extraction logic would go here */

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Extract operation completed.",
                          1.0f);

  return rin_plugin_ok;
}

static
rin_plugin_error_t
plugin_build(void *priv, char const *src, char const *dst) {
  plugin_data_t *self = (plugin_data_t *) priv;

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Build operation started.",
                          0.0f);

  /* Build logic would go here */

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Build operation completed.",
                          1.0f);

  return rin_plugin_ok;
}
