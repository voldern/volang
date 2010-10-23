#include <stdarg.h>
#include "vl.h"

OBJ VlArray_new() {
  VlArray *a = VL_MALLOC(VlArray);
  kv_init(a->kv);
  return (OBJ)a;
}

OBJ VlArray_new2(int argc, ...) {
  OBJ a = VlArray_new();
  va_list argp;
  int i;

  va_start(argp, argc);
  for (i = 0; i < argc; ++i)
    VL_ARRAY_PUSH(a, va_arg(argp, OBJ));
  va_end(argp);
  return a;
}
