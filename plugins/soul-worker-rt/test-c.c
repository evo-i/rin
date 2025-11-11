#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#endif
#include "../../src/rin-plugin-api.h"

#include "soul_worker_rt_private.h"

uint64_t
prefixed_wstring_aggregate(const prefixed_wstring_t* pstr) {
  uint64_t hash = 0;

  for (uint16_t i = 0; i < pstr->length; i++) {
    uint8_t *ptr = (uint8_t *)&pstr->value[i];
    hash += ptr[0] + ptr[1];
  }

  return hash;
}

uint64_t
achievement_data_aggregate(const achievement_data_t* achievement) {
  uint64_t hash = 0;
  hash += prefixed_wstring_aggregate(&achievement->title);
  hash += prefixed_wstring_aggregate(&achievement->description);
  hash += achievement->id;
  return hash;
}

uint64_t
achievement_table_aggregate(const achievement_table_t* table) {
  uint64_t hash = 0;
  for (uint32_t i = 0; i < table->count; i++) {
    hash += achievement_data_aggregate(&table->achievements[i]);
  }
  return hash += table->count;
}

size_t
read_prefixed_wstring(FILE* file, prefixed_wstring_t* pstr) {
  size_t total_bytes = 0;

  // Read length
  if (fread(&pstr->length, sizeof(uint16_t), 1, file) != 1) {
    return 0;
  }

  total_bytes += sizeof(uint16_t);

  // Allocate and read value
  pstr->value = malloc(pstr->length * sizeof(uint16_t));

  if (!pstr->value) {
    return 0;
  }

  if (fread(pstr->value, sizeof(uint16_t), pstr->length, file) != pstr->length) {
    free(pstr->value);
    return 0;
  }

  total_bytes += pstr->length * sizeof(uint16_t);

  return total_bytes;
}

size_t
read_prefixed_string(FILE* file, prefixed_string_t* pstr) {
  size_t total_bytes = 0;

  // Read length
  if (fread(&pstr->length, sizeof(uint16_t), 1, file) != 1) {
    return 0;
  }

  total_bytes += sizeof(uint16_t);

  // Allocate and read value (+1 for null terminator)
  pstr->value = malloc(pstr->length * sizeof(uint8_t) + 1);

  if (!pstr->value) {
    return 0;
  }

  if (fread(pstr->value, sizeof(uint8_t), pstr->length, file) != pstr->length) {
    free(pstr->value);
    return 0;
  }

  pstr->value[pstr->length] = '\0'; // Null-terminate

  total_bytes += pstr->length * sizeof(uint8_t);

  return total_bytes;
}

size_t
read_achievement_data(FILE* file, achievement_data_t* achievement) {
  size_t total_bytes = 0;

  // Read ID
  if (fread(&achievement->id, sizeof(achievement->id), 1, file) != 1) {
    return 0;
  }

  total_bytes += sizeof(achievement->id);

  // Read title
  if (read_prefixed_wstring(file, &achievement->title) == 0) {
    return 0;
  }

  total_bytes += sizeof(achievement->title);

  // Read description
  if (read_prefixed_wstring(file, &achievement->description) == 0) {
    return 0;
  }

  total_bytes += sizeof(achievement->description);

  return total_bytes;
}

size_t
read_table_hash(FILE* file, achievement_table_t* achievement) {
  return read_prefixed_string(file, &achievement->hash);
}

uint64_t
prefixed_string_hash(const prefixed_string_t* pstr) {
  uint64_t hash = 0;

  for (uint16_t i = 0; i < pstr->length; i++) {
    hash += pstr->value[i];
  }

  return hash;
}

#ifdef _WIN32
// Compute MD5 hash from a string and return as lowercase hex
static BOOL compute_md5_from_string(const char* str, char out_hex[33]) {
  if (!str || !out_hex) return FALSE;
  size_t len = strlen(str);

  HCRYPTPROV hProv = 0;
  HCRYPTHASH hHash = 0;
  BOOL       ok    = CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
  if (!ok)
    return FALSE;
  ok = CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
  if (!ok) {
    CryptReleaseContext(hProv, 0);
    return FALSE;
  }
  if (!CryptHashData(hHash, (const BYTE*)str, (DWORD)len, 0))
    ok = FALSE;

  unsigned char md5bin[16];
  DWORD         dlen = 16;
  if (ok && !CryptGetHashParam(hHash, HP_HASHVAL, md5bin, &dlen, 0))
    ok = FALSE;
  if (hHash) CryptDestroyHash(hHash);
  if (hProv) CryptReleaseContext(hProv, 0);
  if (!ok)
    return FALSE;

  static const char* HX = "0123456789abcdef";
  for (int i = 0; i < 16; ++i) {
    out_hex[i * 2]     = HX[(md5bin[i] >> 4) & 0xF];
    out_hex[i * 2 + 1] = HX[md5bin[i] & 0xF];
  }
  SecureZeroMemory(md5bin, sizeof(md5bin)); // или volatile memset
  out_hex[32] = '\0';
  return TRUE;
}
#endif

int
main(int argc, char* argv[]) {
  const char* table_path =
      "F:\\msys64\\home\\devbi\\rin\\build\\packages\\swae-1.0.0-"
      "win64\\bin\\extracted\\data11\\bin\\Table\\tb_Achievement_Script_ENG.res";
  achievement_table_t achievement_table = {0};
  FILE*               file              = fopen(table_path, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open achievement table file\n");
    return 1;
  }

  // Read count
  if (fread(&achievement_table.count, sizeof(uint32_t), 1, file) != 1) {
    fprintf(stderr, "Failed to read achievement count\n");
    fclose(file);
    return 1;
  }

  // Allocate achievements array
  achievement_table.achievements = malloc(achievement_table.count * sizeof(achievement_data_t));
  if (!achievement_table.achievements) {
    fprintf(stderr, "Failed to allocate achievements array\n");
    fclose(file);
    return 1;
  }

  for (uint32_t i = 0; i < achievement_table.count; i++) {
    if (read_achievement_data(file, &achievement_table.achievements[i]) == 0) {
      fprintf(stderr, "Failed to read achievement data at index %u\n", i);
      fclose(file);
      return 1;
    }
  }

  // Read hash
  size_t hash_size = read_table_hash(file, &achievement_table);
  if (hash_size == 0) {
    fprintf(stderr, "Failed to read achievement table hash\n");
    fclose(file);
    return 1;
  }

  fclose(file);

  // Print achievements data
  printf("Achievement Table\n");
  printf("=================\n");
  printf("Count: %u\n\n", achievement_table.count);

  // for (uint32_t i = 0; i < achievement_table.count; i++) {
  //   achievement_data_t* ach = &achievement_table.achievements[i];
  //   printf("Achievement #%u:\n", i + 1);
  //   printf("  ID: %u\n", ach->id);

  //   // Print title (UTF-16LE)
  //   printf("  Title (%u units): ", ach->title.length);
  //   for (uint16_t j = 0; j < ach->title.length; j++) {
  //     if (ach->title.value[j] < 128) {
  //       printf("%c", (char)ach->title.value[j]);
  //     } else {
  //       printf("[U+%04X]", ach->title.value[j]);
  //     }
  //   }
  //   printf("\n");

  //   // Print description (UTF-16LE)
  //   printf("  Description (%u units): ", ach->description.length);
  //   for (uint16_t j = 0; j < ach->description.length; j++) {
  //     if (ach->description.value[j] < 128) {
  //       printf("%c", (char)ach->description.value[j]);
  //     } else {
  //       printf("[U+%04X]", ach->description.value[j]);
  //     }
  //   }
  //   printf("\n\n");
  // }

  // Print hash
  // printf("Hash: ");
  // if (achievement_table.hash.length > 0) {
  //   // Print first 16 bytes of hash (or less if smaller)
  //   for (int i = 0; i < achievement_table.hash.length; i++) {
  //     printf("%02x ", achievement_table.hash.value[i]);
  //   }
  //   printf("...\n");
  // } else {
  //   printf("(none)\n");
  // }

  /* ======================= MD5 VERIFICATION TEST ======================= */

  uint64_t aggregate = achievement_table_aggregate(&achievement_table);
  char     md5hex[33];

  char buffer[256] = {0};

  snprintf(buffer, sizeof(buffer), "%" PRIu64, aggregate);

  compute_md5_from_string(buffer, md5hex);
  printf("Computed MD5:      %s\n", md5hex);
  printf("Stored   MD5:      %s\n", achievement_table.hash.value);

  /* ===================== END MD5 VERIFICATION TEST ===================== */

  // Cleanup
  for (uint32_t i = 0; i < achievement_table.count; i++) {
    free(achievement_table.achievements[i].title.value);
    free(achievement_table.achievements[i].description.value);
  }
  free(achievement_table.hash.value);
  free(achievement_table.achievements);
  return 0;
}
