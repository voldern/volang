#include "vl.h"
#include "opcode.h"

#define PUSH_OP(BLK,I) ({ \
  printf("Pushing op: %X\n", (I)); \
  kv_push(VlInst, (BLK)->code, (I)); \
  kv_size(BLK->code)-1; \
})

#define REG(R) if ((size_t)R >= c->regc) c->regc = (size_t)R+1;

#define PUSH_OP_A(BLK, OP, A) PUSH_OP(BLK, CREATE_ABC(VL_OP_##OP, A, 0, 0))
#define PUSH_OP_AB(BLK, OP, A, B) PUSH_OP(BLK, CREATE_ABC(VL_OP_##OP, A, B, 0))
#define PUSH_OP_ABC(BLK, OP, A, B, C) PUSH_OP(BLK, CREATE_ABC(VL_OP_##OP, A, B, C))
#define PUSH_OP_ABx(BLK, OP, A, Bx) PUSH_OP(BLK, CREATE_ABx(VL_OP_##OP, A, Bx))

#define NODE_ARG(N,I) (((VlNode *)N)->args[I])
#define NODE_TYPE(N) (((VlNode *)N)->ntype)

void VlNode_debug(VlNode *node);

OBJ VlNode_new(VM, VlNodeType type, OBJ a, OBJ b, OBJ c) {
  VlNode *n = VL_MALLOC(VlNode);
  n->ntype = type;
  n->args[0] = a;
  n->args[1] = b;
  n->args[2] = c;

  /* if (vm->debug) { */
  /*   VlNode_debug(n); */
  /* } */
  
  return (OBJ)n;
}

void VlNode_debug(VlNode *node) {
  switch (NODE_TYPE(node)) {
  case NODE_VALUE:
    printf("value %d\n", (int)NODE_ARG(node, 0));
    break;
  case NODE_ADD:
    printf("add %d %d\n", (int)NODE_ARG(NODE_ARG(node, 0), 0),
           (int)NODE_ARG(NODE_ARG(node, 1), 0));
    break;
  case NODE_SETCONST:
    printf("setconst %s ", VL_STR_PTR(NODE_ARG(node, 0)));
    VlNode_debug((VlNode *)NODE_ARG(node, 1));
    break;
  default:
    printf("Unknown node type %d\n", NODE_TYPE(node));
  }
}

VlCompiler *VlCompiler_new(VM) {
  VlCompiler *c = VL_MALLOC(VlCompiler);
  c->line = 1;
  c->vm = vm;
  /* c->block = VlBlock_new(c, 0); */
  c->reg = 0;
  c->regc = 0;
  c->argc = 0;
  c->node = (OBJ)0;
  
  kv_init(c->k);
  kv_init(c->code);
  
  return c;
}

void VlCompiler_compile(VlCompiler *compiler) {
  if (compiler->vm->debug) {
    printf("Compiling!\n");
  }
  
  if (kv_size(((VlArray*)compiler->node)->kv) != 0) {
    size_t i;
    for (i = 0; i < kv_size(((VlArray*)compiler->node)->kv); i++) {
      OBJ n = kv_A(((VlArray*)compiler->node)->kv, i);
      VlCompile_node(compiler->vm, compiler, n, 0);
    }
  }
}

void VlCompile_node(VM, VlCompiler *c, OBJ a, int reg) {
  REG(reg)
  REG(reg)
  
  if (NODE_TYPE(a) == NODE_VALUE) {
    int i = VlBlock_push_value(c, NODE_ARG(a, 0));
    PUSH_OP_ABx(c, LOADK, reg, i);
  } else if (NODE_TYPE(a) == NODE_SETCONST) {
    char *str = VL_STR_PTR(NODE_ARG(a, 0));
    int val = (int)NODE_ARG(NODE_ARG(a, 1), 0);
    
    printf("Setting: %s = %d\n", str, val);

    REG(reg);
    VlCompile_node(vm, c, NODE_ARG(a, 1), reg);
    PUSH_OP_ABx(c, SETCONST, reg, VlBlock_push_value(c, NODE_ARG(a, 0)));
    
  } else if (NODE_TYPE(a) == NODE_GETCONST) {
    /* char *str = VL_STR_PTR(NODE_ARG(a, 0)); */
    /* OBJ value = VlObject_const_get(vm, (OBJ)NODE_ARG(a, 0)); */
    /* printf("Getting %s = %d\n", str, (int) (NODE_ARG(value, 0))); */
    PUSH_OP_ABx(c, GETCONST, reg, VlBlock_push_value(c, NODE_ARG(a, 0)));
  } else if (NODE_TYPE(a) == NODE_ADD) {
    VlNode *rcv = (VlNode *)NODE_ARG(a, 0);
    VlNode *msg = (VlNode *)NODE_ARG(a, 1);

    if (NODE_TYPE(rcv) != NODE_VALUE || NODE_TYPE(msg) != NODE_VALUE) {
      printf("Error: Only integer multiplication is currently supported.\n");
      return;
    }

    int num1 = NODE_ARG(rcv, 0);
    int num2 = NODE_ARG(msg, 0);

    int rcvVal = VlBlock_push_value(c, num1) | 0x100;
    int argVal = VlBlock_push_value(c, num2) | 0x100;

    REG(reg+1)
    
    printf("Multiplication: %d + %d = %d\n", num1, num2, num1 + num2);
    PUSH_OP_ABC(c, ADD, reg, rcvVal, argVal);
  } else {
    printf("Unknown node type %d!\n", NODE_TYPE(a));
  }
}
