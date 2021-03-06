#include "vl.h"
#include "opcode.h"

#define PUSH_OP(BLK,I) ({                       \
      printf("Pushing op: %X\n", (I));          \
      kv_push(VlInst, (BLK)->code, (I));        \
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
  kv_init(c->locals);
  kv_init(c->code);
  
  return c;
}

unsigned int VlCompiler_compile(VlCompiler *compiler) {
  if (compiler->vm->debug) {
    printf("Compiling!\n");
  }

  if ((VlArray *)compiler->node == NULL) {
    printf("Error: empty AST\n");
    return 1;
  }

  if (kv_size(((VlArray*)compiler->node)->kv) != 0) {
    size_t i;
    for (i = 0; i < kv_size(((VlArray*)compiler->node)->kv); i++) {
      OBJ n = kv_A(((VlArray*)compiler->node)->kv, i);
      VlCompile_node(compiler->vm, compiler, n, 0);
    }
  }

  return 0;
}

void VlCompile_node(VM, VlCompiler *c, OBJ a, int reg) {
  REG(reg);
  int rcvVal;
  int argVal;
  VlNode *rcv;
  VlNode *msg;

  switch (NODE_TYPE(a)) {
  case NODE_VALUE:
    PUSH_OP_ABx(c, LOADK, reg, VlBlock_push_value(c, NODE_ARG(a, 0)));
    break;
  case NODE_SETCONST:
    REG(reg);
    VlCompile_node(vm, c, NODE_ARG(a, 1), reg);
    PUSH_OP_ABx(c, SETCONST, reg, VlBlock_push_value(c, NODE_ARG(a, 0)));
    break;
  case NODE_GETCONST:
    PUSH_OP_ABx(c, GETCONST, reg, VlBlock_push_value(c, NODE_ARG(a, 0)));
    break;
  case NODE_ASSIGN: {
    REG(reg);
    VlCompile_node(vm, c, NODE_ARG(a, 1), reg);

    int i = VlBlock_push_local(c, (OBJ)NODE_ARG(a, 0));
    if (i != reg) {
      PUSH_OP_AB(c, MOVE, i, reg);
    }
  } break;
  case NODE_ADD:
  case NODE_SUB:
  case NODE_MUL:
  case NODE_DIV:
  case NODE_MOD:
    rcv = (VlNode *)NODE_ARG(a, 0);
    msg = (VlNode *)NODE_ARG(a, 1);

    if (NODE_TYPE(rcv) == NODE_VALUE) {
      rcvVal = VlBlock_push_value(c, NODE_ARG(rcv, 0)) | 0x100;
    } else {
      REG(reg);
      VlCompile_node(vm, c, (OBJ)rcv, reg);
      rcvVal = reg;
    }

    if (NODE_TYPE(msg) == NODE_VALUE) {
      argVal = VlBlock_push_value(c, NODE_ARG(msg, 0)) | 0x100;
    } else {
      REG(reg+1);
      VlCompile_node(vm, c, (OBJ)msg, reg+1);
      argVal = reg + 1;
    }

    REG(reg+1);

    switch (NODE_TYPE(a)) {
    case NODE_ADD: PUSH_OP_ABC(c, ADD, reg, rcvVal, argVal); break;
    case NODE_SUB: PUSH_OP_ABC(c, SUB, reg, rcvVal, argVal); break;
    case NODE_MUL: PUSH_OP_ABC(c, MUL, reg, rcvVal, argVal); break;
    case NODE_DIV: PUSH_OP_ABC(c, DIV, reg, rcvVal, argVal); break;
    case NODE_MOD: PUSH_OP_ABC(c, MOD, reg, rcvVal, argVal); break;
    }
    break;
  case NODE_RETURN:
    PUSH_OP_A(c, DOH, NODE_ARG(a, 0));
    break;
  case NODE_INVOKE: {
    int i = VlBlock_find_local(c, (OBJ)NODE_ARG(a, 0));
    printf("move %s %d\n", VL_STR_PTR(NODE_ARG(a, 0)), i);
    
    if (i != -1 && reg != i) {
      PUSH_OP_AB(c, MOVE, reg, i);
    } else {
      printf("could not find %s\t", VL_STR_PTR(NODE_ARG(a, 0)));
    }
  } break;
  default:
    printf("Unknown node type %d!\n", NODE_TYPE(a));
  }
}
