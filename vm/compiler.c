#include "vl.h"

#define PUSH_OP(BLK,I) ({ \
  kv_push(VlInst, (BLK)->code, (I)); \
  kv_size(BLK->code)-1; \
})

#define PUSH_OP_A(BLK, OP, A)         PUSH_OP(BLK, CREATE_ABC(TR_OP_##OP, A, 0, 0))
#define PUSH_OP_AB(BLK, OP, A, B)     PUSH_OP(BLK, CREATE_ABC(TR_OP_##OP, A, B, 0))
#define PUSH_OP_ABC(BLK, OP, A, B, C) PUSH_OP(BLK, CREATE_ABC(TR_OP_##OP, A, B, C))
#define PUSH_OP_ABx(BLK, OP, A, Bx)   PUSH_OP(BLK, CREATE_ABx(TR_OP_##OP, A, Bx))

#define NODE_ARG(N,I) (((VlNode *)N)->args[I])
#define NODE_TYPE(N) (((VlNode *)N)->ntype)

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

void VlCompile_node(VM, VlBlock *b, OBJ a) {
  if (NODE_TYPE(a) == NODE_VALUE) {
    int i = VlBlock_push_value(b, NODE_ARG(a, 0));
    PUSH_OP_ABx(b, LOADK, reg, i);
  } else if (NODE_TYPE(a) == NODE_SETCONST) {
    char *str = VL_STR_PTR(NODE_ARG(a, 0));
    int val = (int)NODE_ARG(NODE_ARG(a, 1), 0);
    
    printf("Setting: %s = %d\n", str, val);
    VlObject_const_set(vm, (OBJ)NODE_ARG(a, 0), (OBJ)NODE_ARG(a, 1));
  } else if (NODE_TYPE(a) == NODE_GETCONST) {
    char *str = VL_STR_PTR(NODE_ARG(a, 0));
    OBJ value = VlObject_const_get(vm, (OBJ)NODE_ARG(a, 0));
    
    printf("Getting %s = %d\n", str, (int) (NODE_ARG(value, 0)));
  } else if (NODE_TYPE(a) == NODE_ADD) {
    VlNode *rcv = (VlNode *)NODE_ARG(a, 0);
    VlNode *msg = (VlNode *)NODE_ARG(a, 1);

    if (NODE_TYPE(rcv) != NODE_VALUE || NODE_TYPE(msg) != NODE_VALUE) {
      printf("Error: Only integer multiplication is currently supported.\n");
      return;
    }

    int num1 = NODE_ARG(rcv, 0);
    int num2 = NODE_ARG(msg, 0);
    
    printf("Multiplication: %d + %d = %d\n", num1, num2, num1 + num2);
  } else {
    printf("Unknown node type %d!\n", NODE_TYPE(a));
  }
}

