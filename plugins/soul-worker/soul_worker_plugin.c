#include "soul_worker_private.h"

#include <zip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static
char *
local_get_full_path(const char *path) {
  DWORD size = GetFullPathNameA(path, 0, NULL, NULL);
  if (size == 0) {
    return NULL;
  }

  char *full_path = (char *) malloc(size);
  if (!full_path) {
    return NULL;
  }

  if (GetFullPathNameA(path, size, full_path, NULL) == 0) {
    free(full_path);
    return NULL;
  }

  return full_path;
}
static
char **
local_get_files_list(const char *dir_name, size_t *out_count) {
  char search_path[MAX_PATH];
  snprintf(search_path, MAX_PATH, "%s\\*", dir_name);
  WIN32_FIND_DATAA find_data;
  HANDLE hFind = FindFirstFileA(search_path, &find_data);
  if (hFind == INVALID_HANDLE_VALUE) {
    return NULL;
  }
  char **file_list = NULL;
  size_t count = 0;
  do {
    if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      char *full_path = (char *) malloc(MAX_PATH);
      if (!full_path) {
        continue;
      }
      snprintf(full_path, MAX_PATH, "%s\\%s", dir_name, find_data.cFileName);
      char **new_list = (char **) realloc(file_list, sizeof(char *) * (count + 1));
      if (!new_list) {
        free(full_path);
        continue;
      }
      file_list = new_list;
      file_list[count] = full_path;
      count++;
    }
  } while (FindNextFileA(hFind, &find_data) != 0);
  FindClose(hFind);
  *out_count = count;
  return file_list;
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
static
char *
local_get_full_path(const char *path) {
  char *full_path = realpath(path, NULL);
  return full_path;
}
static
char **
local_get_files_list(const char *dir_name, size_t *out_count) {
  if (!dir_name) {
    *out_count = 0;
    return NULL;
  }

  DIR *dir = opendir(dir_name);
  if (!dir) {
    return NULL;
  }

  char **file_list = NULL;
  size_t count = 0;

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      char *full_path = (char *) malloc(4096);
      if (!full_path) {
        continue;
      }
      snprintf(full_path, 4096, "%s/%s", dir_name, entry->d_name);
      char **new_list = (char **) realloc(file_list, sizeof(char *) * (count + 1));
      if (!new_list) {
        free(full_path);
        continue;
      }
      file_list = new_list;
      file_list[count] = full_path;
      count++;
    }
  }

  closedir(dir);
  *out_count = count;
  return file_list;
}
#endif

static rin_plugin_info_t const *
plugin_get_info(void);

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
                      | rin_plugin_caps_can_extract)
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
  rin_plugin_callbacks_t *cb = self->callbacks;
  if (self) {
    for (size_t i = 0; i < self->passwords.count; ++i) {
      free(self->passwords.pairs[i].key);
      free(self->passwords.pairs[i].value);
    }
    free(self->passwords.pairs);
    free(self);
  }
  cb->report(cb->user_data,
             rin_plugin_report_info,
             "Plugin instance destroyed.",
             0.0f);
}

bool
plugin_can_handle_file(void *priv, char const *file_name) {
  uint8_t header[4] = {0};
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

  // Load passwords immediately when we detect a valid directory
  if (self->passwords.count == 0) {
    char *full_path = local_get_full_path(dir_name);
    if (full_path) {
      plugin_context_t ctx;
      ctx.data = *self;
      if (plugin_get_passwords(&ctx, full_path, &self->passwords)) {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_info,
                                "Passwords loaded successfully.",
                                0.0f);
      } else {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_warning,
                                "Failed to load passwords (will retry during extraction).",
                                0.0f);
      }
      free(full_path);
    }
  }

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Directory can be handled by the plugin.",
                          0.0f);
  return true;
}

static
zip_int64_t
local_zip_source_layered_callback(zip_source_t *src, void *user_data, void *data, zip_uint64_t len, enum zip_source_cmd cmd) {
  switch (cmd) {
  case ZIP_SOURCE_READ: {
    uint8_t *buf = (uint8_t *) data;
    // Read from lower layer into provided buffer
    zip_int64_t res = zip_source_pass_to_lower_layer(src, data, len, cmd);
    if (res > 0) {
      for (zip_int64_t i = 0; i < res; ++i)
        buf[i] ^= 0x55;
    }
    return res;
  }
  case ZIP_SOURCE_WRITE: {
    uint8_t *buf = (uint8_t *) data;
    for (zip_int64_t i = 0; i < (zip_int64_t)len; ++i)
      buf[i] ^= 0x55;
    // Write transformed buffer to lower layer
    return zip_source_pass_to_lower_layer(src, data, len, cmd);
  }
  default:
    return zip_source_pass_to_lower_layer(src, data, len, cmd);
  }
}

#include <errno.h>
#include <sys/types.h>

static const char *
local_basename(const char *path) {
  if (!path) return "";
  const char *slash1 = strrchr(path, '/');
#ifdef _WIN32
  const char *slash2 = strrchr(path, '\\');
  const char *p = slash1 && slash2 ? (slash1 > slash2 ? slash1 : slash2)
                                   : (slash1 ? slash1 : slash2);
#else
  const char *p = slash1;
#endif
  return p ? (p + 1) : path;
}

static const char *
local_basename_no_ext(const char *path) {
  const char *base = local_basename(path);
  const char *dot = strrchr(base, '.');
  if (dot) {
    // Allocate a copy without extension
    static char buf[256]; // assuming max filename length
    size_t len = dot - base;
    if (len < sizeof(buf)) {
      memcpy(buf, base, len);
      buf[len] = '\0';
      return buf;
    }
  }
  return base; // no extension or too long
}

static bool
local_path_has_traversal(const char *relpath) {
  if (!relpath) return false;
  if (relpath[0] == '/'
#ifdef _WIN32
      || relpath[0] == '\\'
#endif
  ) return true;
  // naive traversal check
  return strstr(relpath, "../") != NULL || strstr(relpath, "..\\") != NULL;
}

static const char *
normalize_path(const char *path) {
  static char buf[4096];
  if (!path || strlen(path) >= sizeof(buf)) return path;
  strcpy(buf, path);
  char *p = buf;
  while (1) {
    if (p[0] == '.' && p[1] == '.' && (p[2] == '/'
#ifdef _WIN32
                                       || p[2] == '\\'
#endif
                                      )) {
      p += 3;
    } else if (p[0] == '.' && p[1] == '.' && p[2] == '\0') {
      *p = '\0';
      break;
    } else {
      break;
    }
  }
  return p;
}

static bool
local_mkdir_single(const char *path) {
#ifdef _WIN32
  if (CreateDirectoryA(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
    return true;
  }
  return false;
#else
  if (mkdir(path, 0755) == 0 || errno == EEXIST) {
    return true;
  }
  return false;
#endif
}

static bool
local_mkdir_p(const char *path) {
  if (!path || !*path) return false;
  char tmp[4096];
  size_t len = strlen(path);
  if (len >= sizeof(tmp)) return false;
  memcpy(tmp, path, len + 1);

  // normalize separators on Windows for splitting only
#ifdef _WIN32
  for (size_t i = 0; i < len; ++i) if (tmp[i] == '\\') tmp[i] = '/';
#endif

  // iterate components
  for (size_t i = 1; i <= len; ++i) {
    if (tmp[i] == '/' || tmp[i] == '\0') {
      char save = tmp[i];
      tmp[i] = '\0';
      if (strlen(tmp) > 0) {
        if (!local_mkdir_single(tmp)) {
          // another process may have created it in between; try stat later if needed
          // continue only if already exists
#ifndef _WIN32
          if (errno != EEXIST)
            return false;
#else
          DWORD err = GetLastError();
          if (err != ERROR_ALREADY_EXISTS)
            return false;
#endif
        }
      }
      tmp[i] = save;
    }
  }
  return true;
}

static bool
ensure_parent_dirs(const char *filepath) {
  char buf[4096];
  size_t len = strlen(filepath);
  if (len >= sizeof(buf)) return false;
  memcpy(buf, filepath, len + 1);

  // find last separator
  char *last_slash = strrchr(buf, '/');
#ifdef _WIN32
  char *last_back = strrchr(buf, '\\');
  char *p = last_slash && last_back ? (last_slash > last_back ? last_slash : last_back)
                                    : (last_slash ? last_slash : last_back);
#else
  char *p = last_slash;
#endif
  if (!p) return true; // no parent
  *p = '\0';
  return local_mkdir_p(buf);
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
  char *full_src_path = local_get_full_path(src);
  if (!full_src_path) {
    self->callbacks->report(self->callbacks->user_data,
                            rin_plugin_report_error,
                            "Failed to resolve source path.",
                            0.0f);
    return rin_plugin_error_io;
  }

  // Load passwords if not loaded (fallback if can_handle_dir wasn't called)
  if (self->passwords.count == 0) {
    char sw_path[4096];
    strcpy(sw_path, full_src_path);
    char *last_slash = strrchr(sw_path, '/');
    if (last_slash) *last_slash = '\0'; // remove trailing slash and last component
    plugin_context_t ctx;
    ctx.data = *self;
    if (!plugin_get_passwords(&ctx, sw_path, &self->passwords)) {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_warning,
                              "Failed to load passwords.",
                              0.0f);
    }
  }

  // Check for range filter from environment variables
  int start_index = -1;
  int end_index = -1;
  const char *env_start = getenv("SWAE_START_INDEX");
  const char *env_end = getenv("SWAE_END_INDEX");
  
  if (env_start) {
    start_index = atoi(env_start);
  }
  if (env_end) {
    end_index = atoi(env_end);
  }

  size_t files_count = 0;
  char **files_list = local_get_files_list(full_src_path, &files_count);
  zip_error_t zip_err;
  zip_error_init(&zip_err);

  size_t processed = 0;
  size_t total_to_process = 0;
  
  // Count files to process
  for (size_t i = 0; i < files_count; ++i) {
    const char *basename = local_basename(files_list[i]);
    int file_index = -1;
    
    // Extract index from filename (e.g., data05.v -> 5)
    // Only count .v files (exact match)
    size_t name_len = strlen(basename);
    if (name_len > 2 && basename[name_len - 2] == '.' && basename[name_len - 1] == 'v') {
      if (sscanf(basename, "data%d.v", &file_index) == 1) {
        if ((start_index < 0 || file_index >= start_index) &&
            (end_index < 0 || file_index <= end_index)) {
          total_to_process++;
        }
      }
    }
  }

  for (size_t i = 0; i < files_count; ++i) {
    const char *basename = local_basename(files_list[i]);
    int file_index = -1;
    
    // Extract index from filename (e.g., data05.v -> 5)
    // Only process .v files (exact match)
    size_t name_len = strlen(basename);
    if (name_len > 2 && basename[name_len - 2] == '.' && basename[name_len - 1] == 'v') {
      if (sscanf(basename, "data%d.v", &file_index) == 1) {
        // Check if file is in the requested range
        if (start_index >= 0 && file_index < start_index) {
          continue; // Skip files before start
        }
        if (end_index >= 0 && file_index > end_index) {
          continue; // Skip files after end
        }
      } else {
        // .v file but not matching data##.v pattern - skip
        continue;
      }
    } else {
      // Not a .v file - skip
      continue;
    }

    processed++;
    self->callbacks->report(self->callbacks->user_data,
                            rin_plugin_report_info,
                            files_list[i],
                            total_to_process > 0 ? (float)processed / (float)total_to_process : 0.0f);

    // Decide whether to apply XOR layer based on header
    bool use_layer = false;
    {
      unsigned char hdr[4] = {0};
      FILE *f = fopen(files_list[i], "rb");
      if (f) {
        size_t rb = fread(hdr, 1, sizeof(hdr), f);
        fclose(f);
        if (rb == sizeof(hdr)) {
          uint32_t hv = (hdr[0] | (hdr[1] << 8) | (hdr[2] << 16) | (hdr[3] << 24));
          if (hv == v_value) use_layer = true;
          else if (hv == z_value) use_layer = false;
        }
      }
    }

    zip_source_t *za = zip_source_file_create(files_list[i], 0, ZIP_LENGTH_TO_END, &zip_err);
    if (!za) {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_error,
                              "Failed to create zip source.",
                              0.0f);
      continue;
    }

    zip_source_t *zs = use_layer
      ? zip_source_layered_create(za, local_zip_source_layered_callback, NULL, NULL)
      : za;

    if (!zs) {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_error,
                              "Failed to create layered zip source.",
                              0.0f);
      zip_source_free(za);
      continue;
    }

    zip_t *zt = zip_open_from_source(zs, 0, 0);
    if (!zt) {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_error,
                              "Failed to open zip from source.",
                              0.0f);
      zip_source_free(zs);
      continue;
    }

    // Create output directory: dst/<archive name without extension>
    const char *archive_name = local_basename_no_ext(files_list[i]);
    char out_dir[4096];
    snprintf(out_dir, sizeof(out_dir), "%s/%s", dst, archive_name);
    if (!local_mkdir_p(out_dir)) {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_error,
                              "Failed to create output directory.",
                              0.0f);
      zip_close(zt);
      continue;
    }

    // Apply password if present
    const char *password_key = local_basename_no_ext(files_list[i]);
    const char *password = NULL;
    for (size_t pi = 0; pi < self->passwords.count; ++pi) {
      if (self->passwords.pairs[pi].key && strcmp(self->passwords.pairs[pi].key, password_key) == 0) {
        password = self->passwords.pairs[pi].value;
        break;
      }
    }
    if (password && zip_set_default_password(zt, password) < 0) {
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_warning,
                              "Failed to set password, proceeding without.",
                              0.0f);
    }

    // Extract entries
    zip_int64_t n_entries = zip_get_num_entries(zt, 0);
    size_t extracted = 0;
    for (zip_uint64_t idx = 0; idx < (zip_uint64_t)n_entries; ++idx) {
      struct zip_stat st;
      if (zip_stat_index(zt, idx, 0, &st) != 0) {
        continue;
      }
      if (!st.name) continue;
      const char *normalized = normalize_path(st.name);
      if (!normalized || !*normalized) continue;
      char out_path[4096];
      if (snprintf(out_path, sizeof(out_path), "%s/%s", out_dir, normalized) >= (int)sizeof(out_path)) {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_warning,
                                "Output path too long, skipping entry.",
                                0.0f);
        continue;
      }

      size_t name_len = strlen(normalized);
      bool is_dir = name_len > 0 && (normalized[name_len - 1] == '/'
#ifdef _WIN32
                                      || st.name[name_len - 1] == '\\'
#endif
      );
      if (is_dir) {
        (void) local_mkdir_p(out_path);
        continue;
      }

      if (!ensure_parent_dirs(out_path)) {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_warning,
                                "Failed to create parent directories.",
                                0.0f);
        continue;
      }

      zip_file_t *zf = zip_fopen_index(zt, idx, 0);
      if (!zf) {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_warning,
                                "Failed to open entry from archive.",
                                0.0f);
        continue;
      }

      char msg[4096];
      snprintf(msg, sizeof(msg), "Extracting from %s: %s", archive_name, normalized);
      self->callbacks->report(self->callbacks->user_data,
                              rin_plugin_report_progress,
                              msg,
                              (float)(extracted + 1) / (float)n_entries);

      FILE *of = fopen(out_path, "wb");
      if (!of) {
        self->callbacks->report(self->callbacks->user_data,
                                rin_plugin_report_warning,
                                "Failed to create output file.",
                                0.0f);
        zip_fclose(zf);
        continue;
      }
      uint8_t buf[64 * 1024];
      zip_int64_t r;
      while ((r = zip_fread(zf, buf, sizeof(buf))) > 0) {
        if (fwrite(buf, 1, (size_t)r, of) != (size_t)r) {
          self->callbacks->report(self->callbacks->user_data,
                                  rin_plugin_report_warning,
                                  "Short write while extracting.",
                                  0.0f);
          break;
        }
      }
      zip_fclose(zf);
      fclose(of);
      extracted++;
    }

    // Close archive (also releases zip source)
    zip_close(zt);
  }
  free(files_list);
  free(full_src_path);

  self->callbacks->report(self->callbacks->user_data,
                          rin_plugin_report_info,
                          "Extract operation completed.",
                          1.0f);

  return rin_plugin_ok;
}

static
rin_plugin_error_t
plugin_build(void *priv, char const *src, char const *dst) {
  // plugin_data_t *self = (plugin_data_t *) priv;

  // self->callbacks->report(self->callbacks->user_data,
  //                         rin_plugin_report_info,
  //                         "Build operation started.",
  //                         0.0f);

  // /* Build logic would go here */

  // self->callbacks->report(self->callbacks->user_data,
  //                         rin_plugin_report_info,
  //                         "Build operation completed.",
  //                         1.0f);

  return rin_plugin_error_unsupported;
}
