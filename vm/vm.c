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

void VlVM_destroy(VM) {
  kh_destroy(OBJ, vm->consts);

  VlSymbol_destroy(vm);

  VL_FREE(vm);
}

OBJ VlVM_const_get(VM, OBJ name) {
  khiter_t k = kh_get(OBJ, vm->consts, name);
  
  if (k != kh_end(vm->consts)) {
    return kh_value(vm->consts, k);
  }

  return (OBJ)0;
}

OBJ VlVM_const_set(VM, OBJ name, OBJ value) {
  int ret;
  khiter_t k;
  k = kh_put(OBJ, vm->consts, name, &ret);
  //if (!ret) kh_del(OBJ, vm->consts, k);
  kh_value(vm->consts, k) = value;
  k = 0;
  k = kh_get(OBJ, vm->consts, name);

  return value;
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
    case VL_OP_MOVE:
      printf("\tmove\n");
      stack[GETARG_A(i)] = stack[GETARG_B(i)];
      break;
    case VL_OP_LOADK:
      printf("\tload r%d %d\n", (int)GETARG_Bx(i), (int)k[GETARG_Bx(i)]);
      stack[GETARG_A(i)] = k[GETARG_Bx(i)];
      i = *++ip;
      break;
    case VL_OP_GETCONST:
      stack[GETARG_A(i)] = VlVM_const_get(compiler->vm, k[GETARG_Bx(i)]);

      printf("\tgetconst %s %d\n", VL_STR_PTR(k[GETARG_Bx(i)]), (int)stack[GETARG_A(i)]);
      i = *++ip;
      break;
    case VL_OP_SETCONST:
      VlVM_const_set(compiler->vm, k[GETARG_Bx(i)], stack[GETARG_A(i)]);
      printf("\tsetconst %s %d\n", VL_STR_PTR(k[GETARG_Bx(i)]), (int)stack[GETARG_A(i)]);
      i = *++ip;
      break;
    case VL_OP_ADD:
      printf("\tadd r%d %d %d\n", GETARG_A(i), (int)RK(GETARG_B(i)), (int)RK(GETARG_C(i)));

      stack[GETARG_A(i)] = RK(GETARG_B(i)) + RK(GETARG_C(i));
      i = *++ip;
      break;
    case VL_OP_SUB:
      printf("\tsub r%d %d %d\n", GETARG_A(i), (int)RK(GETARG_B(i)), (int)RK(GETARG_C(i)));

      stack[GETARG_A(i)] = RK(GETARG_B(i)) - RK(GETARG_C(i));
      i = *++ip;
      break;
    case VL_OP_MUL:
      printf("\tmul r%d %d %d\n", GETARG_A(i), (int)RK(GETARG_B(i)), (int)RK(GETARG_C(i)));

      stack[GETARG_A(i)] = RK(GETARG_B(i)) * RK(GETARG_C(i));
      i = *++ip;
      break;
    case VL_OP_DIV:
      printf("\tdiv r%d %d %d\n", GETARG_A(i), (int)RK(GETARG_B(i)), (int)RK(GETARG_C(i)));

      stack[GETARG_A(i)] = RK(GETARG_B(i)) / RK(GETARG_C(i));
      i = *++ip;
      break;
    case VL_OP_MOD:
      printf("\tmod r%d %d %d\n", GETARG_A(i), (int)RK(GETARG_B(i)), (int)RK(GETARG_C(i)));

      stack[GETARG_A(i)] = RK(GETARG_B(i)) % RK(GETARG_C(i));
      i = *++ip;
      break;
    default:
      printf("Unknown opcode: %d\n", (int)GET_OPCODE(i));
      return;
    }
  }
}
