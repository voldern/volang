#ifndef _OPCODE_H_
#define _OPCODE_H_

#include "vl.h"

/* Taken from Tinyrb which in turn took it from Lua */
#define cast(T,X) ((T)X)

#define SIZE_C    9
#define SIZE_B    9
#define SIZE_Bx   (SIZE_C + SIZE_B)
#define SIZE_A    8

#define SIZE_OP   6

#define POS_OP    0
#define POS_A     (POS_OP + SIZE_OP)
#define POS_C     (POS_A + SIZE_A)
#define POS_B     (POS_C + SIZE_C)
#define POS_Bx    POS_C

// Bx is unsigned
#define MAXARG_Bx   ((1<<SIZE_Bx)-1)

/* creates a mask with `n' 1 bits at position `p' */
#define MASK1(n,p)  ((~((~(VlInst)0)<<n))<<p)

/* creates a mask with `n' 0 bits at position `p' */
#define MASK0(n,p)  (~MASK1(n,p))

/* the following macros help to manipulate instructions (VlInst) */
#define GET_OPCODE(i) (cast(int, ((i)>>POS_OP) & MASK1(SIZE_OP,POS_OP)))
#define SET_OPCODE(i,o) ((i) = (((i)&MASK0(SIZE_OP,POS_OP)) | \
    ((cast(VlInst, o)<<POS_OP)&MASK1(SIZE_OP,POS_OP))))

#define GETARG_A(i) (cast(int, ((i)>>POS_A) & MASK1(SIZE_A,0)))
#define SETARG_A(i,u) ((i) = (((i)&MASK0(SIZE_A,POS_A)) | \
    ((cast(VlInst, u)<<POS_A)&MASK1(SIZE_A,POS_A))))

#define GETARG_B(i) (cast(int, ((i)>>POS_B) & MASK1(SIZE_B,0)))
#define SETARG_B(i,b) ((i) = (((i)&MASK0(SIZE_B,POS_B)) | \
    ((cast(VlInst, b)<<POS_B)&MASK1(SIZE_B,POS_B))))

#define GETARG_C(i) (cast(int, ((i)>>POS_C) & MASK1(SIZE_C,0)))
#define SETARG_C(i,b) ((i) = (((i)&MASK0(SIZE_C,POS_C)) | \
    ((cast(VlInst, b)<<POS_C)&MASK1(SIZE_C,POS_C))))

#define GETARG_Bx(i)  (cast(int, ((i)>>POS_Bx) & MASK1(SIZE_Bx,0)))
#define SETARG_Bx(i,b)  ((i) = (((i)&MASK0(SIZE_Bx,POS_Bx)) | \
    ((cast(VlInst, b)<<POS_Bx)&MASK1(SIZE_Bx,POS_Bx))))

#define CREATE_ABC(o,a,b,c) ((cast(VlInst, o)<<POS_OP) \
      | (cast(VlInst, a)<<POS_A) \
      | (cast(VlInst, b)<<POS_B) \
      | (cast(VlInst, c)<<POS_C))

#define CREATE_ABx(o,a,bc)  ((cast(VlInst, o)<<POS_OP) \
      | (cast(VlInst, a)<<POS_A) \
      | (cast(VlInst, bc)<<POS_Bx))

enum VlInstCode {
    VL_OP_DOH,
    VL_OP_MOVE,       /* A B      R[A] = R[B]  */
    VL_OP_LOADK,      /* A Bx     R[A] = K[Bx] */
    VL_OP_GETCONST,   /* A Bx     R[A] = Consts[k[Bx]] */
    VL_OP_SETCONST,   /* A Bx     Consts[k[Bx]] = R[A] */
    VL_OP_ADD,        /* A B C    R[A] = RK[B] + RK[C] */
    VL_OP_SUBTRACT        /* A B C    R[A] = RK[B] + RK[C] */
};

#endif
