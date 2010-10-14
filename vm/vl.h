#ifndef _VOLANG_H_
#define _VOLANG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long OBJ;

typedef enum {
  NODE_CONST,
  NODE_VALUE,
  NODE_ASSIGN,
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


OBJ VlNode_new(VlNodeType type, OBJ a, OBJ b, OBJ c);
OBJ VlSymbol_new(const char *str);
void VlCompile(OBJ a);

#define NODE(T,A)	VlNode_new(NODE_##T, (A), 0, 0)
#define NODE2(T,A,B)	VlNode_new(NODE_##T, (A), (B), 0)
#define COMPILE(A)      VlCompile(A)

#define VL_MALLOC(T) (T *)malloc(sizeof(T))
#define VL_MALLOC_N(T,N) (T *)calloc((N), sizeof(N))

void VlBlock_compile(char *code);

#endif /* _VOLANG_H_ */
