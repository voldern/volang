#include "vl.h"

OBJ VlObject_const_get(VM, OBJ name) {
  khiter_t k = kh_get(OBJ, vm->consts, name);
  printf("GETTING OBJ = %d\n", (int)name);

  if (k != kh_end(vm->consts)) {
    printf("GETTING OBJ = %d\n", (int)kh_value(vm->consts, k));
    return kh_value(vm->consts, k);
  }

  return (OBJ)0;
}

OBJ VlObject_const_set(VM, OBJ name, OBJ value) {
  int ret;
  khiter_t k;
  k = kh_put(OBJ, vm->consts, name, &ret);
  //if (!ret) kh_del(OBJ, vm->consts, k);
  kh_value(vm->consts, k) = value;
  k = 0;
  k = kh_get(OBJ, vm->consts, name);

  printf("SETTING OBJ = %d\n", (int)name);

  return value;
}