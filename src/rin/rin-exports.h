#ifndef RIN_EXPORTS_H
#define RIN_EXPORTS_H 1

#if defined(_WIN32) || defined(_WIN64)
  #define RIN_PLATFORM_WINDOWS 1
#elif defined(__linux__)
  #define RIN_PLATFORM_LINUX 1
#elif defined(__APPLE__)
  #define RIN_PLATFORM_APPLE 1
#else
  #define RIN_PLATFORM_UNKNOWN 1
#endif

#if defined(_MSC_VER)
  #define RIN_COMPILER_MSVC 1
#elif defined(__GNUC__)
  #define RIN_COMPILER_GCC 1
#elif defined(__clang__)
  #define RIN_COMPILER_CLANG 1
#else
  #define RIN_COMPILER_UNKNOWN 1
#endif

#if defined(RIN_PLATFORM_WINDOWS)
  #if defined(RIN_SHARED_BUILD) || defined(__rin_plugin__)
    #if defined(RIN_EXPORTS) || defined(__rin_plugin__)
      #define RIN_API __declspec(dllexport)
    #else
      #define RIN_API __declspec(dllimport)
    #endif
  #else
    #define RIN_API
  #endif
  #define RIN_LOCAL
#else
  #if __GNUC__ >= 4 || defined(__clang__)
    #define RIN_API __attribute__((visibility("default")))
    #define RIN_LOCAL __attribute__((visibility("hidden")))
  #else
    #define RIN_API
    #define RIN_LOCAL
  #endif
#endif

// Fallback definitions if not defined elsewhere
#ifndef RIN_API
  #define RIN_API
#endif

#ifndef RIN_LOCAL
  #define RIN_LOCAL
#endif

#endif /* RIN_EXPORTS_H */
