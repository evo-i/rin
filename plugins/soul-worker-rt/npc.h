#ifndef NPC_H
#define NPC_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "prefixed_string.h"

struct npc {
  uint32_t                id;
  struct prefixed_wstring name;
};

struct npc *
npc_new(void);

size_t
npc_read(FILE *file, struct npc *self);

size_t
npc_write(FILE *file, struct npc const *self);

void
npc_free(struct npc *self);

size_t
npc_hash(struct npc const *self);

size_t
npc_size(struct npc const *self);

#ifdef __cplusplus
}
#endif

#endif
