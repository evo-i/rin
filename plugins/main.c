#include <windows.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path_to_dll>\n", argv[0]);
    return 1;
  }

  const char* dll_path = argv[1];
  HMODULE     module   = LoadLibraryA(dll_path);
  if (!module) {
    fprintf(stderr, "Failed to load DLL: %s\n", dll_path);
    return 1;
  }

  // Successfully loaded the DLL
  printf("Successfully loaded DLL: %s\n", dll_path);

  // Perform any necessary operations with the loaded DLL here

  // Free the loaded DLL
  FreeLibrary(module);
  return 0;
}
