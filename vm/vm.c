#include "vl.h"

VlVM *VlVM_new() {
  VlVM *vm = VL_MALLOC(VlVM);
  vm->consts = kh_init(OBJ);
  vm->debug = 0;

  return vm;
}
