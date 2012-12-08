#ifndef _VOLANG_H_
#define _VOLANG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vendor/khash.h"
#include "vendor/kvec.h"

typedef unsigned long OBJ;
typedef unsigned int VlInst;

KHASH_MAP_INIT_STR(str, OBJ);
KHASH_MAP_INIT_INT(OBJ, OBJ);

typedef enum {
  NODE_CONST,
  NODE_VALUE,
  NODE_ASSIGN,
  NODE_SETCONST,
  NODE_GETCONST,
  NODE_SEND,
  NODE_ADD
} VlNodeType;

typedef struct {
  VlNodeType ntype;
  OBJ args[3];
} VlNode;

typedef struct {
  char *ptr;

  size_t len;
} VlSymbol;

typedef struct {
  kvec_t(OBJ) kv;
} VlArray;

typedef struct VlVM {
  khash_t(str) *symbols;
  khash_t(OBJ) *consts;

  int debug;
  
} VlVM;

#define VM	struct VlVM *vm

OBJ VlNode_new(VM, VlNodeType type, OBJ a, OBJ b, OBJ c);
OBJ VlSymbol_new(VM, const char *str);
void VlCompile(VM, OBJ a);
void VlCompile_node(VM, OBJ a);

/* vm */
VlVM *VlVM_new();
void VlVM_destroy(VlVM *vm);

/* object */
OBJ VlObject_const_get(VM, OBJ name);
OBJ VlObject_const_set(VM, OBJ name, OBJ value);

/* array */
OBJ VlArray_new();
OBJ VlArray_new2(int argc, ...);
#define VL_ARRAY_PUSH(X, I)	kv_push(OBJ, ((VlArray*)(X))->kv, (I))

#define NODE(T,A)	VlNode_new(yyvm, NODE_##T, (A), 0, 0)
#define NODE2(T,A,B)	VlNode_new(yyvm, NODE_##T, (A), (B), 0)
#define NODES(I)	VlArray_new2(1, (I));
#define PUSH_NODE(A,N)	VL_ARRAY_PUSH((A),(N))
#define COMPILE(A,B)      VlCompile((A),(B))

#define VL_MALLOC(T) (T *)malloc(sizeof(T))
#define VL_MALLOC_N(T,N) (T *)calloc((N), sizeof(N))
#define VL_FREE(P) free(P)

#define TR_IS_FIX(F) ((F) & 1)
#define VL_STR_PTR(S) (((VlSymbol *)S)->ptr)

void VlBlock_compile(char *code, int debug);

#endif /* _VOLANG_H_ */
