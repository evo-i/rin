#include "cargs.h"
#include "plugin_loader.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define PATH_SEPARATOR "\\"
#define mkdir(path, mode) _mkdir(path)
#define stat _stat
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#else
#include <dirent.h>
#include <unistd.h>
#define PATH_SEPARATOR "/"
#endif

static struct cag_option options[] = {
    {.identifier     = 'o',
     .access_letters = "o",
     .access_name    = "output",
     .value_name     = "DIR",
     .description    = "Output directory for extracted files"},
    {.identifier     = 's',
     .access_letters = "s",
     .access_name    = "start",
     .value_name     = "NUM",
     .description    = "Start index for file range (e.g., data00.v -> 0)"},
    {.identifier     = 'e',
     .access_letters = "e",
     .access_name    = "end",
     .value_name     = "NUM",
     .description    = "End index for file range (e.g., data11.v -> 11)"},
    {.identifier     = 'i',
     .access_letters = "i",
     .access_name    = "interactive",
     .description    = "Interactive mode - select .v files to extract"},
    {.identifier     = 'h',
     .access_letters = "h",
     .access_name    = "help",
     .description    = "Show help message"}};

static int
file_exists(const char* path) {
  struct stat st;
  return stat(path, &st) == 0;
}

static int
is_directory(const char* path) {
  struct stat st;
  if (stat(path, &st) != 0) {
    return 0;
  }
  return S_ISDIR(st.st_mode);
}

static char*
path_join(const char* path1, const char* path2) {
  size_t len1     = strlen(path1);
  size_t len2     = strlen(path2);
  int    need_sep = (len1 > 0 && path1[len1 - 1] != '/' && path1[len1 - 1] != '\\');

  size_t total_len = len1 + len2 + (need_sep ? 1 : 0) + 1;
  char*  result    = malloc(total_len);
  if (!result) {
    return NULL;
  }

  strcpy(result, path1);
  if (need_sep) {
    strcat(result, PATH_SEPARATOR);
  }
  strcat(result, path2);

  return result;
}

static int
mkdir_recursive(const char* path) {
  char*  p    = NULL;
  char*  temp = strdup(path);
  size_t len  = strlen(temp);

  if (!temp) {
    return -1;
  }

  if (temp[len - 1] == '/' || temp[len - 1] == '\\') {
    temp[len - 1] = '\0';
  }

  // Try to create the directory
  if (mkdir(path, 0755) == 0) {
    free(temp);
    return 0;
  }

  if (errno == EEXIST) {
    free(temp);
    return 0;
  }

  for (p = temp + 1; *p; p++) {
    if (*p == '/' || *p == '\\') {
      *p = '\0';
      if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
        free(temp);
        return -1;
      }
      *p = PATH_SEPARATOR[0];
    }
  }

  int result = mkdir(temp, 0755);
  free(temp);

  if (result != 0 && errno != EEXIST) {
    return -1;
  }

  return 0;
}

static char*
get_current_dir(void) {
#ifdef _WIN32
  char* buf = malloc(MAX_PATH);
  if (!buf) {
    return NULL;
  }
  if (GetCurrentDirectoryA(MAX_PATH, buf) == 0) {
    free(buf);
    return NULL;
  }
  return buf;
#else
  return getcwd(NULL, 0);
#endif
}

// Structure to hold .v file information
typedef struct {
  char* name;
  int   index;
  long  size;
} v_file_info_t;

// Compare function for sorting v_file_info_t by index
static int
compare_v_files(const void* a, const void* b) {
  const v_file_info_t* fa = (const v_file_info_t*)a;
  const v_file_info_t* fb = (const v_file_info_t*)b;
  return fa->index - fb->index;
}

// Scan directory for .v files and return sorted list (excluding data00.v)
static v_file_info_t*
scan_v_files(const char* datas_dir, size_t* out_count) {
  if (!datas_dir || !out_count) {
    return NULL;
  }

  v_file_info_t* files    = NULL;
  size_t         count    = 0;
  size_t         capacity = 0;

#ifdef _WIN32
  char* search_pattern = path_join(datas_dir, "data*.v");
  if (!search_pattern) {
    return NULL;
  }

  WIN32_FIND_DATAA find_data;
  HANDLE           hFind = FindFirstFileA(search_pattern, &find_data);
  free(search_pattern);

  if (hFind == INVALID_HANDLE_VALUE) {
    *out_count = 0;
    return NULL;
  }

  do {
    // Parse filename: data##.v (exact match, not .vArc or other extensions)
    int    index;
    size_t len = strlen(find_data.cFileName);

    // Must end with exactly ".v" (not ".vArc" or anything else)
    if (len > 2 && find_data.cFileName[len - 2] == '.' && find_data.cFileName[len - 1] == 'v' &&
        sscanf(find_data.cFileName, "data%d.v", &index) == 1) {

      // Skip data00.v
      if (index == 0) {
        continue;
      }

      // Expand array if needed
      if (count >= capacity) {
        capacity                 = capacity == 0 ? 16 : capacity * 2;
        v_file_info_t* new_files = realloc(files, capacity * sizeof(v_file_info_t));
        if (!new_files) {
          for (size_t i = 0; i < count; i++) {
            free(files[i].name);
          }
          free(files);
          FindClose(hFind);
          return NULL;
        }
        files = new_files;
      }

      files[count].name  = strdup(find_data.cFileName);
      files[count].index = index;
      files[count].size  = ((long long)find_data.nFileSizeHigh << 32) | find_data.nFileSizeLow;
      count++;
    }
  } while (FindNextFileA(hFind, &find_data));

  FindClose(hFind);

#else
  DIR* dir = opendir(datas_dir);
  if (!dir) {
    *out_count = 0;
    return NULL;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    // Parse filename: data##.v (exact match, not .vArc or other extensions)
    int    index;
    size_t len = strlen(entry->d_name);

    // Must end with exactly ".v" (not ".vArc" or anything else)
    if (len > 2 && entry->d_name[len - 2] == '.' && entry->d_name[len - 1] == 'v' &&
        sscanf(entry->d_name, "data%d.v", &index) == 1) {

      // Skip data00.v
      if (index == 0) {
        continue;
      }

      // Get file size
      char*       full_path = path_join(datas_dir, entry->d_name);
      struct stat st;
      long        size = 0;
      if (full_path && stat(full_path, &st) == 0) {
        size = st.st_size;
      }
      free(full_path);

      // Expand array if needed
      if (count >= capacity) {
        capacity                 = capacity == 0 ? 16 : capacity * 2;
        v_file_info_t* new_files = realloc(files, capacity * sizeof(v_file_info_t));
        if (!new_files) {
          for (size_t i = 0; i < count; i++) {
            free(files[i].name);
          }
          free(files);
          closedir(dir);
          return NULL;
        }
        files = new_files;
      }

      files[count].name  = strdup(entry->d_name);
      files[count].index = index;
      files[count].size  = size;
      count++;
    }
  }

  closedir(dir);
#endif

  // Sort by index
  if (count > 0) {
    qsort(files, count, sizeof(v_file_info_t), compare_v_files);
  }

  *out_count = count;
  return files;
}

// Free v_file_info_t array
static void
free_v_files(v_file_info_t* files, size_t count) {
  if (!files) {
    return;
  }
  for (size_t i = 0; i < count; i++) {
    free(files[i].name);
  }
  free(files);
}

// Interactive file selection
static int
interactive_select_range(const v_file_info_t* files, size_t count, int* out_start, int* out_end) {
  if (!files || count == 0 || !out_start || !out_end) {
    return 0;
  }

  printf("\n========================================\n");
  printf("Available .v files in datas directory:\n");
  printf("========================================\n");
  printf("  #  | Index | Filename       | Size\n");
  printf("-----+-------+----------------+----------\n");

  for (size_t i = 0; i < count; i++) {
    double size_mb = files[i].size / (1024.0 * 1024.0);
    printf("%4zu | %5d | %-14s | %7.2f MB\n", i + 1, files[i].index, files[i].name, size_mb);
  }

  printf("========================================\n\n");

  // Ask for range
  char input[256];
  int  start_idx = -1, end_idx = -1;

  printf("Select range to extract:\n");
  printf("  - Enter 'all' to extract all files\n");
  printf("  - Enter single number (e.g., '5') to extract one file\n");
  printf("  - Enter range (e.g., '1-5' or '1 5') to extract multiple files\n");
  printf("\nYour choice: ");

  if (!fgets(input, sizeof(input), stdin)) {
    return 0;
  }

  // Remove trailing newline
  input[strcspn(input, "\n")] = 0;

  // Parse input
  if (strcmp(input, "all") == 0) {
    *out_start = files[0].index;
    *out_end   = files[count - 1].index;
    return 1;
  }

  // Try to parse as range: "1-5" or "1 5"
  if (sscanf(input, "%d-%d", &start_idx, &end_idx) == 2 ||
      sscanf(input, "%d %d", &start_idx, &end_idx) == 2) {
    // Validate indices
    if (start_idx < 1 || start_idx > (int)count || end_idx < 1 || end_idx > (int)count ||
        start_idx > end_idx) {
      fprintf(stderr, "Invalid range. Please enter numbers between 1 and %zu.\n", count);
      return 0;
    }
    *out_start = files[start_idx - 1].index;
    *out_end   = files[end_idx - 1].index;
    return 1;
  }

  // Try to parse as single number
  if (sscanf(input, "%d", &start_idx) == 1) {
    if (start_idx < 1 || start_idx > (int)count) {
      fprintf(stderr, "Invalid number. Please enter a number between 1 and %zu.\n", count);
      return 0;
    }
    *out_start = files[start_idx - 1].index;
    *out_end   = files[start_idx - 1].index;
    return 1;
  }

  fprintf(stderr, "Invalid input format.\n");
  return 0;
}

// Scan plugins directory and find a plugin that can handle the directory
static plugin_t*
find_and_load_plugin(const char* plugins_dir, const char* target_dir) {
#ifdef _WIN32
  WIN32_FIND_DATAA find_data;
  char*            search_pattern = path_join(plugins_dir, "*");
  if (!search_pattern) {
    return NULL;
  }

  HANDLE hFind = FindFirstFileA(search_pattern, &find_data);
  free(search_pattern);

  if (hFind == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Failed to scan plugins directory: %s\n", plugins_dir);
    return NULL;
  }

  plugin_t* found_plugin = NULL;

  do {
    // Skip . and ..
    if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) {
      continue;
    }

    // Check if it's a directory
    char* subdir = path_join(plugins_dir, find_data.cFileName);
    if (!subdir)
      continue;

    if (is_directory(subdir)) {
      // Look for .dll files in this subdirectory
      char* dll_pattern = path_join(subdir, "*.dll");
      if (dll_pattern) {
        WIN32_FIND_DATAA dll_data;
        HANDLE           hDll = FindFirstFileA(dll_pattern, &dll_data);
        free(dll_pattern);

        if (hDll != INVALID_HANDLE_VALUE) {
          do {
            // Skip . and ..
            if (strcmp(dll_data.cFileName, ".") == 0 || strcmp(dll_data.cFileName, "..") == 0) {
              continue;
            }

            char* plugin_path = path_join(subdir, dll_data.cFileName);
            if (!plugin_path)
              continue;

            // Try to load the plugin
            plugin_t* plugin = plugin_new();
            if (plugin && plugin_load(plugin, plugin_path)) {
              // Check if this plugin can handle the target directory
              if (plugin_can_handle_dir(plugin, target_dir)) {
                printf("Found suitable plugin: %s\n", plugin_get_name(plugin));
                free(plugin_path);
                FindClose(hDll);
                free(subdir);
                FindClose(hFind);
                return plugin;
              }
              plugin_free(plugin);
            } else if (plugin) {
              plugin_free(plugin);
            }

            free(plugin_path);
          } while (FindNextFileA(hDll, &dll_data));
          FindClose(hDll);
        }
      }
    }
    free(subdir);
  } while (FindNextFileA(hFind, &find_data));

  FindClose(hFind);

#else
  DIR* dir = opendir(plugins_dir);
  if (!dir) {
    fprintf(stderr, "Failed to open plugins directory: %s\n", plugins_dir);
    return NULL;
  }

  plugin_t*      found_plugin = NULL;
  struct dirent* entry;

  while ((entry = readdir(dir)) != NULL) {
    // Skip . and ..
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char* subdir = path_join(plugins_dir, entry->d_name);
    if (!subdir)
      continue;

    if (is_directory(subdir)) {
      // Look for .so files in this subdirectory
      DIR* plugin_dir = opendir(subdir);
      if (plugin_dir) {
        struct dirent* plugin_entry;

        while ((plugin_entry = readdir(plugin_dir)) != NULL) {
          // Check if file ends with .so
          size_t len = strlen(plugin_entry->d_name);
          if (len > 3 && strcmp(plugin_entry->d_name + len - 3, ".so") == 0) {
            char* plugin_path = path_join(subdir, plugin_entry->d_name);
            if (!plugin_path)
              continue;

            // Try to load the plugin
            plugin_t* plugin = plugin_new();
            if (plugin && plugin_load(plugin, plugin_path)) {
              // Check if this plugin can handle the target directory
              if (plugin_can_handle_dir(plugin, target_dir)) {
                printf("Found suitable plugin: %s\n", plugin_get_name(plugin));
                free(plugin_path);
                closedir(plugin_dir);
                free(subdir);
                closedir(dir);
                return plugin;
              }
              plugin_free(plugin);
            } else if (plugin) {
              plugin_free(plugin);
            }

            free(plugin_path);
          }
        }
        closedir(plugin_dir);
      }
    }
    free(subdir);
  }

  closedir(dir);
#endif

  return NULL;
}

int
main(int argc, char* argv[]) {
  cag_option_context context;
  const char*        game_folder = NULL;
  const char*        output_dir  = NULL;
  int                start_index = -1;
  int                end_index   = -1;
  int                interactive = 0;

  // Parse command line arguments
  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);

  while (cag_option_fetch(&context)) {
    char identifier = cag_option_get(&context);
    switch (identifier) {
    case 'o':
      output_dir = cag_option_get_value(&context);
      break;
    case 's':
      start_index = atoi(cag_option_get_value(&context));
      break;
    case 'e':
      end_index = atoi(cag_option_get_value(&context));
      break;
    case 'i':
      interactive = 1;
      break;
    case 'h':
      printf("Usage: %s [OPTIONS] <game_folder_path>\n", argv[0]);
      printf("Extract Soul Worker archives\n\n");
      printf("Options:\n");
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      printf("\nExamples:\n");
      printf("  %s -i /path/to/game              Interactive mode - select files\n", argv[0]);
      printf("  %s /path/to/game                 Extract all files\n", argv[0]);
      printf("  %s -s 1 -e 5 /path/to/game       Extract data01.v to data05.v\n", argv[0]);
      printf("  %s -o /output /path/to/game      Extract to custom directory\n", argv[0]);
      return 0;
    }
  }

  if (context.index < argc) {
    game_folder = argv[context.index];
  }

  if (!game_folder) {
    fprintf(stderr, "Usage: %s [OPTIONS] <game_folder_path>\n", argv[0]);
    fprintf(stderr, "Run '%s --help' for more information.\n", argv[0]);
    return 1;
  }

  if (!is_directory(game_folder)) {
    fprintf(stderr, "Error: %s is not a valid directory\n", game_folder);
    return 1;
  }

  // Check for conflicting options
  if (interactive && (start_index >= 0 || end_index >= 0)) {
    fprintf(stderr, "Error: Cannot use -i (interactive) with -s/-e options\n");
    fprintf(stderr, "Use either interactive mode OR specify range manually.\n");
    return 1;
  }

  // Validate range if specified
  if (start_index >= 0 && end_index >= 0 && start_index > end_index) {
    fprintf(stderr, "Error: start index (%d) cannot be greater than end index (%d)\n", start_index,
            end_index);
    return 1;
  }

  char* current_dir = get_current_dir();
  if (!current_dir) {
    fprintf(stderr, "Failed to get current directory\n");
    return 1;
  }

  char* plugins_dir = path_join(current_dir, "plugins");
  free(current_dir);

  if (!plugins_dir) {
    fprintf(stderr, "Failed to build plugins path\n");
    return 1;
  }

  // Check if plugins directory exists
  if (!is_directory(plugins_dir)) {
    fprintf(stderr, "Plugins directory not found: %s\n", plugins_dir);
    free(plugins_dir);
    return 1;
  }

  // Find and load a suitable plugin based on game_folder (not datas subdirectory)
  printf("Scanning plugins directory: %s\n", plugins_dir);
  plugin_t* plugin = find_and_load_plugin(plugins_dir, game_folder);
  free(plugins_dir);

  if (!plugin) {
    fprintf(stderr, "No suitable plugin found for: %s\n", game_folder);
    return 1;
  }

  // Set source to game_folder/datas for extraction
  char* src = path_join(game_folder, "datas");
  if (!src) {
    fprintf(stderr, "Failed to build source path\n");
    plugin_free(plugin);
    return 1;
  }

  // Check if src exists and is a directory
  if (!is_directory(src)) {
    fprintf(stderr, "Error: %s is not a valid directory\n", src);
    free(src);
    plugin_free(plugin);
    return 1;
  }

  // Interactive mode: scan and select files
  if (interactive) {
    size_t         v_count = 0;
    v_file_info_t* v_files = scan_v_files(src, &v_count);

    if (!v_files || v_count == 0) {
      fprintf(stderr, "No .v files found in %s (excluding data00.v)\n", src);
      free(src);
      plugin_free(plugin);
      return 1;
    }

    if (!interactive_select_range(v_files, v_count, &start_index, &end_index)) {
      fprintf(stderr, "File selection cancelled or invalid.\n");
      free_v_files(v_files, v_count);
      free(src);
      plugin_free(plugin);
      return 1;
    }

    free_v_files(v_files, v_count);

    printf("\nSelected range: data%02d.v to data%02d.v\n\n", start_index, end_index);
  }

  // Set destination
  char* dest;
  if (output_dir) {
    dest = strdup(output_dir);
  } else {
    dest = path_join(game_folder, "extracted");
  }

  if (!dest) {
    fprintf(stderr, "Failed to build destination path\n");
    free(src);
    plugin_free(plugin);
    return 1;
  }

  // Ensure dest directory exists
  if (mkdir_recursive(dest) != 0) {
    fprintf(stderr, "Failed to create destination directory: %s\n", dest);
    free(src);
    free(dest);
    plugin_free(plugin);
    return 1;
  }

  // Print extraction info
  if (start_index >= 0 || end_index >= 0) {
    // Set environment variables for plugins to use
    char buf[32];
    if (start_index >= 0) {
      snprintf(buf, sizeof(buf), "%d", start_index);
#ifdef _WIN32
      _putenv_s("SWAE_START_INDEX", buf);
#else
      setenv("SWAE_START_INDEX", buf, 1);
#endif
    }
    if (end_index >= 0) {
      snprintf(buf, sizeof(buf), "%d", end_index);
#ifdef _WIN32
      _putenv_s("SWAE_END_INDEX", buf);
#else
      setenv("SWAE_END_INDEX", buf, 1);
#endif
    }

    if (start_index >= 0 && end_index >= 0) {
      printf("Extracting archives data%02d.v to data%02d.v from %s to %s...\n", start_index,
             end_index, src, dest);
    } else if (start_index >= 0) {
      printf("Extracting archives starting from data%02d.v from %s to %s...\n", start_index, src,
             dest);
    } else {
      printf("Extracting archives up to data%02d.v from %s to %s...\n", end_index, src, dest);
    }
  } else {
    printf("Extracting all archives from %s to %s...\n", src, dest);
  }

  // Extract
  if (!plugin_extract(plugin, src, dest)) {
    fprintf(stderr, "Extraction failed\n");
    free(src);
    free(dest);
    plugin_free(plugin);
    return 1;
  }

  printf("Extraction completed successfully.\n");

  free(src);
  free(dest);
  plugin_free(plugin);
  return 0;
}
