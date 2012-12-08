#include "vl.h"
#include "opcode.h"

VlBlock *VlBlock_new() {
  VlBlock *b = VL_MALLOC(VlBlock);
  kv_init(b->k);
  kv_init(b->code);
  kv_init(b->locals);
  b->regc = 0;
  b->argc = 0;
  b->line = 1;
  return b;
}

int VlBlock_push_value(VlCompiler *blk, OBJ k) {
  size_t i;
  for (i = 0; i < kv_size(blk->k); ++i)
    if (kv_A(blk->k, i) == k) return i;
  
  kv_push(OBJ, blk->k, k);
  return kv_size(blk->k)-1;
}
