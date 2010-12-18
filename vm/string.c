#include "vl.h"

static OBJ VlSymbol_lookup(VM, const char *str) {
  khash_t(str) *kh = vm->symbols;
  khiter_t k = kh_get(str, kh, str);
  
  if (k != kh_end(kh))
    return kh_value(kh, k);
  
  return (OBJ)0;
}

static void VlSymbol_add(VM, const char *str, OBJ id) {
  int ret;
  khash_t(str) *kh = vm->symbols;
  khiter_t k = kh_put(str, kh, str, &ret);
  
  if (!ret)
    kh_del(str, kh, k);
  
  kh_value(kh, k) = id;
}

OBJ VlSymbol_new(VM, const char *str) {
  OBJ id = VlSymbol_lookup(vm, str);
  if (!id) {
  
    VlSymbol *s = VL_MALLOC(VlSymbol);
    s->len = strlen(str);
    s->ptr = VL_MALLOC_N(char, s->len+1);
    memcpy(s->ptr, str, sizeof(char) * s->len);
    s->ptr[s->len] = '\0';

    id = (OBJ)s;
    VlSymbol_add(vm, s->ptr, id);

    if (vm->debug == 1)
      printf("CREATING STRING: %s\n", s->ptr);
  }

  return id;
}


