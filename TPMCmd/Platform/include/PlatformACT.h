// This file contains the definitions for the ACT macros and data types used in the
// ACT implementation.

#ifndef _PLATFORM_ACT_H_
#define _PLATFORM_ACT_H_

typedef struct ACT_DATA
{
    uint32_t remaining;
    uint32_t newValue;
    uint8_t  signaled;
    uint8_t  pending;
    uint8_t  number;
} ACT_DATA, *P_ACT_DATA;

#if !(defined RH_ACT_0) || (RH_ACT_0 != YES)
#  undef RH_ACT_0
#  define RH_ACT_0 NO
#  define IF_ACT_0_IMPLEMENTED(op)
#else
#  define IF_ACT_0_IMPLEMENTED(op) op(0)
#endif
#if !(defined RH_ACT_1) || (RH_ACT_1 != YES)
#  undef RH_ACT_1
#  define RH_ACT_1 NO
#  define IF_ACT_1_IMPLEMENTED(op)
#else
#  define IF_ACT_1_IMPLEMENTED(op) op(1)
#endif
#if !(defined RH_ACT_2) || (RH_ACT_2 != YES)
#  undef RH_ACT_2
#  define RH_ACT_2 NO
#  define IF_ACT_2_IMPLEMENTED(op)
#else
#  define IF_ACT_2_IMPLEMENTED(op) op(2)
#endif
#if !(defined RH_ACT_3) || (RH_ACT_3 != YES)
#  undef RH_ACT_3
#  define RH_ACT_3 NO
#  define IF_ACT_3_IMPLEMENTED(op)
#else
#  define IF_ACT_3_IMPLEMENTED(op) op(3)
#endif
#if !(defined RH_ACT_4) || (RH_ACT_4 != YES)
#  undef RH_ACT_4
#  define RH_ACT_4 NO
#  define IF_ACT_4_IMPLEMENTED(op)
#else
#  define IF_ACT_4_IMPLEMENTED(op) op(4)
#endif
#if !(defined RH_ACT_5) || (RH_ACT_5 != YES)
#  undef RH_ACT_5
#  define RH_ACT_5 NO
#  define IF_ACT_5_IMPLEMENTED(op)
#else
#  define IF_ACT_5_IMPLEMENTED(op) op(5)
#endif
#if !(defined RH_ACT_6) || (RH_ACT_6 != YES)
#  undef RH_ACT_6
#  define RH_ACT_6 NO
#  define IF_ACT_6_IMPLEMENTED(op)
#else
#  define IF_ACT_6_IMPLEMENTED(op) op(6)
#endif
#if !(defined RH_ACT_7) || (RH_ACT_7 != YES)
#  undef RH_ACT_7
#  define RH_ACT_7 NO
#  define IF_ACT_7_IMPLEMENTED(op)
#else
#  define IF_ACT_7_IMPLEMENTED(op) op(7)
#endif
#if !(defined RH_ACT_8) || (RH_ACT_8 != YES)
#  undef RH_ACT_8
#  define RH_ACT_8 NO
#  define IF_ACT_8_IMPLEMENTED(op)
#else
#  define IF_ACT_8_IMPLEMENTED(op) op(8)
#endif
#if !(defined RH_ACT_9) || (RH_ACT_9 != YES)
#  undef RH_ACT_9
#  define RH_ACT_9 NO
#  define IF_ACT_9_IMPLEMENTED(op)
#else
#  define IF_ACT_9_IMPLEMENTED(op) op(9)
#endif
#if !(defined RH_ACT_A) || (RH_ACT_A != YES)
#  undef RH_ACT_A
#  define RH_ACT_A NO
#  define IF_ACT_A_IMPLEMENTED(op)
#else
#  define IF_ACT_A_IMPLEMENTED(op) op(A)
#endif
#if !(defined RH_ACT_B) || (RH_ACT_B != YES)
#  undef RH_ACT_B
#  define RH_ACT_B NO
#  define IF_ACT_B_IMPLEMENTED(op)
#else
#  define IF_ACT_B_IMPLEMENTED(op) op(B)
#endif
#if !(defined RH_ACT_C) || (RH_ACT_C != YES)
#  undef RH_ACT_C
#  define RH_ACT_C NO
#  define IF_ACT_C_IMPLEMENTED(op)
#else
#  define IF_ACT_C_IMPLEMENTED(op) op(C)
#endif
#if !(defined RH_ACT_D) || (RH_ACT_D != YES)
#  undef RH_ACT_D
#  define RH_ACT_D NO
#  define IF_ACT_D_IMPLEMENTED(op)
#else
#  define IF_ACT_D_IMPLEMENTED(op) op(D)
#endif
#if !(defined RH_ACT_E) || (RH_ACT_E != YES)
#  undef RH_ACT_E
#  define RH_ACT_E NO
#  define IF_ACT_E_IMPLEMENTED(op)
#else
#  define IF_ACT_E_IMPLEMENTED(op) op(E)
#endif
#if !(defined RH_ACT_F) || (RH_ACT_F != YES)
#  undef RH_ACT_F
#  define RH_ACT_F NO
#  define IF_ACT_F_IMPLEMENTED(op)
#else
#  define IF_ACT_F_IMPLEMENTED(op) op(F)
#endif

#define FOR_EACH_ACT(op)     \
    IF_ACT_0_IMPLEMENTED(op) \
    IF_ACT_1_IMPLEMENTED(op) \
    IF_ACT_2_IMPLEMENTED(op) \
    IF_ACT_3_IMPLEMENTED(op) \
    IF_ACT_4_IMPLEMENTED(op) \
    IF_ACT_5_IMPLEMENTED(op) \
    IF_ACT_6_IMPLEMENTED(op) \
    IF_ACT_7_IMPLEMENTED(op) \
    IF_ACT_8_IMPLEMENTED(op) \
    IF_ACT_9_IMPLEMENTED(op) \
    IF_ACT_A_IMPLEMENTED(op) \
    IF_ACT_B_IMPLEMENTED(op) \
    IF_ACT_C_IMPLEMENTED(op) \
    IF_ACT_D_IMPLEMENTED(op) \
    IF_ACT_E_IMPLEMENTED(op) \
    IF_ACT_F_IMPLEMENTED(op)

#endif  // _PLATFORM_ACT_H_
