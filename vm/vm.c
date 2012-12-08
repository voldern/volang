#include "vl.h"

VlVM *VlVM_new() {
  VlVM *vm = VL_MALLOC(VlVM);
  vm->consts = kh_init(OBJ);
  vm->symbols = kh_init(str);
  vm->debug = 0;

  /* Initializing commonly used symbols */
  /* VlSymbol_new(vm, "="); */

  return vm;
}

void VlVM_destroy(VlVM *vm) {
  kh_destroy(OBJ, vm->consts);
  kh_destroy(str, vm->symbols);

  VL_FREE(vm);
}
