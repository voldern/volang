#include "vl.h"

OBJ VlNode_new(VM, VlNodeType type, OBJ a, OBJ b, OBJ c) {
  printf("CREATING OBJECT\n");
  VlNode *n = VL_MALLOC(VlNode);
  n->ntype = type;
  n->args[0] = a;
  n->args[1] = b;
  n->args[2] = c;
  
  return (OBJ)n;
}

void VlCompile(VM, OBJ a) {
  //struct VlVM *vm = VlVM_new();
  
  printf("Compiling!\n\n");
  if (kv_size(((VlArray*)a)->kv) != 0) {
    size_t i;
    for (i = 0; i < kv_size(((VlArray*)a)->kv); i++) {
      OBJ n = kv_A(((VlArray*)a)->kv, i);
      VlCompile_node(vm, n);
    }
  }
}

void VlCompile_node(VM, OBJ a) {
  if (((VlNode *)a)->ntype == NODE_SETCONST) {
    char *str = ((VlSymbol *)(((VlNode *)a)->args[0]))->ptr;
    int val = (int)((VlNode *)((VlNode *)a)->args[1])->args[0];
    printf("Setting: %s = %d\n", str, val);
    VlObject_const_set(vm, (OBJ)((VlNode *)a)->args[0], (OBJ)((VlNode *)a)->args[1]);
  } else if (((VlNode *)a)->ntype == NODE_GETCONST) {
    char *str = ((VlSymbol *)(((VlNode *)a)->args[0]))->ptr;
    OBJ value = VlObject_const_get(vm, (OBJ)((VlNode *)a)->args[0]);
    printf("Getting %s = %d\n", str, (int) (((VlNode *)value)->args[0]));
  } else if (((VlNode *)a)->ntype == NODE_ADD) {
    VlNode *rcv = (VlNode *)((VlNode *)a)->args[0];
    VlNode *msg = (VlNode *)((VlNode *)a)->args[1];

    if (rcv->ntype != NODE_VALUE || msg->ntype != NODE_VALUE) {
      printf("Error: Only integer multiplication is currently supported.\n");
      return;
    }

    int num1 = rcv->args[0];
    int num2 = msg->args[0];
    
    printf("Multiplication: %d + %d = %d\n", num1, num2, num1 + num2);
  } else {
    printf("Unknown node type %d!\n", ((VlNode *)a)->ntype);
  }
}

