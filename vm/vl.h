#ifndef _VOLANG_H_
#define _VOLANG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vendor/khash.h"

typedef unsigned long OBJ;

KHASH_MAP_INIT_INT(OBJ, OBJ);

typedef enum {
  NODE_CONST,
  NODE_VALUE,
  NODE_ASSIGN,
  NODE_SETCONST,
  NODE_GETCONST,
  NODE_SEND
} VlNodeType;

typedef struct {
  VlNodeType ntype;
  OBJ args[3];
} VlNode;

typedef struct {
  char *ptr;

  size_t len;
} VlSymbol;

typedef struct VlVM {
  khash_t(OBJ) *consts;

  int debug;
  
} VlVM;

#define VM	struct VlVM *vm

OBJ VlNode_new(VlNodeType type, OBJ a, OBJ b, OBJ c);
OBJ VlSymbol_new(const char *str);
void VlCompile(VM, OBJ a);

/* vm */
VlVM *VlVM_new();

/* object */
OBJ VlObject_const_get(VM, OBJ name);
OBJ VlObject_const_set(VM, OBJ name, OBJ value);

#define NODE(T,A)	VlNode_new(NODE_##T, (A), 0, 0)
#define NODE2(T,A,B)	VlNode_new(NODE_##T, (A), (B), 0)
#define COMPILE(A,B)      VlCompile((A), (B))

#define VL_MALLOC(T) (T *)malloc(sizeof(T))
#define VL_MALLOC_N(T,N) (T *)calloc((N), sizeof(N))

void VlBlock_compile(char *code);

#endif /* _VOLANG_H_ */
