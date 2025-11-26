#ifndef COMMON_H_DEFINED
#define COMMON_H_DEFINED 1

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static inline int
safe_size_add(size_t a, size_t b, size_t *result) {
  if (a > SIZE_MAX - b) {
    return 0; /* Overflow */
  }
  *result = a + b;
  return 1; /* Success */
}

#define CLEANUP_ON_READ_ERROR(cleanup_code) \
  do { \
    cleanup_code; \
    return 0; \
  } while(0)

#define VALIDATE_PTR(ptr) \
  do { \
    if (!(ptr)) { \
      return 0; \
    } \
  } while(0)

#define VALIDATE_READ(condition, cleanup_code) \
  do { \
    if (!(condition)) { \
      CLEANUP_ON_READ_ERROR(cleanup_code); \
    } \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H_DEFINED */
