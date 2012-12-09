#include "vl.h"
#include "opcode.h"

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

#define RK(X)  (X & (1 << (SIZE_B - 1)) ? k[X & ~0x100] : stack[X])

void VlVM_run(VlCompiler *compiler) {
  printf("\n--------------------------------\n"
         "initial objects in stack: %d"
         "\n--------------------------------\nOPS:\n", (int)compiler->regc);
  compiler->stack = alloca(sizeof(OBJ) * compiler->regc);
  OBJ *stack = compiler->stack;
  
  VlInst *ip = compiler->code.a;
  VlInst i = *ip;
  
  OBJ *k = compiler->k.a;

  for (;;) { switch(GET_OPCODE(i)) {
    case VL_OP_DOH:
      printf("\tDOH!\n");
      return;
    case VL_OP_LOADK:
      printf("\tload\n");
      i = *++ip;
      break;
    case VL_OP_ADD:
      printf("\tadd %d %d\n", (int)RK(GETARG_B(i)), (int)RK(GETARG_C(i)));

      stack[GETARG_A(i)] = RK(GETARG_B(i)) + RK(GETARG_C(i));
      i = *++ip;
      break;
    default:
      printf("Unknown opcode: %d\n", (int)GET_OPCODE(i));
      return;
    }
  }
}
