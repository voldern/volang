#include "vl.h"

OBJ VlNode_new(VlNodeType type, OBJ a, OBJ b, OBJ c) {
  printf("CREATING OBJECT\n");
  VlNode *n = VL_MALLOC(VlNode);
  n->ntype = type;
  n->args[0] = a;
  n->args[1] = b;
  n->args[2] = c;
  
  return (OBJ)n;
}

OBJ VlSymbol_new(const char *str) {
  VlSymbol *s = VL_MALLOC(VlSymbol);
  s->len = strlen(str);
  s->ptr = VL_MALLOC_N(char, s->len+1);
  memcpy(s->ptr, str, sizeof(char) * s->len);
  s->ptr[s->len] = '\0';

  printf("CREATING STRING: %s\n", s->ptr);

  return (OBJ)s;
}

void VlCompile(VM, OBJ a) {
  if (!vm) {
    printf("Defining VM\n");
    vm = VlVM_new();
  }
  
  printf("Compiling!\n");
  if (((VlNode *)a)->ntype == NODE_SETCONST) {
    char *str = ((VlSymbol *)(((VlNode *)a)->args[0]))->ptr;
    int val = (int)((VlNode *)((VlNode *)a)->args[1])->args[0];
    printf("Setting: %s = %d\n", str, val);
    VlObject_const_set(vm, (OBJ)((VlNode *)a)->args[0], (OBJ)((VlNode *)a)->args[1]);
  } else if (((VlNode *)a)->ntype == NODE_GETCONST) {
    char *str = ((VlSymbol *)(((VlNode *)a)->args[0]))->ptr;
    printf("Getting: %s ... ", str);
    OBJ value = VlObject_const_get(vm, (OBJ)((VlNode *)a)->args[0]);
    printf("%d", (int)value);
  } else {
    printf("Unknown node type!\n");
  }
}

