#include "fpu.h"

void finit(){
  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fwait;                 "
    ".arch i286             "
  );
}

float fadd(float a, float b){
  float out;

  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fld  %1;               " // %1 -> st0
    "fld  %2;               " // %2 -> st0, %1 -> st1
    "fadd %%st(1), %%st(0); " // st1 + st0 -> st0
    "fstp %0;               " // st0 -> %0 & pop st0
    "fwait;                 "
    ".arch i286             "
    : "=m" (out) : "m" (a), "m" (b)
  );

  return out;
}

int fma_r(float a, float b){
  
  __asm volatile(
    ".arch pentium;         "
    "fld  %0;               " // %1 -> st0
    "fld  %1;               " // %2 -> st0, %1 -> st1
    "fmul %%st(1), %%st(0); " // st1 * st0 -> st0
    "fadd %%st(2), %%st(0); " // fma 
    "fwait;                 "
    ".arch i286             "
    : : "m" (a), "m" (b)
  );
  return 0;
}


int zeroset(){
  float in=0;

  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fld  %0;               " // %1 -> st0
    "fwait;                 "
    ".arch i286             "
    :  : "m" (in)
  );
  return 0;
}

float pop(){
  float out;

  __asm volatile(
    ".arch pentium;         "
    "fstp %0;               " // st0 -> %0 & pop st0
    "fwait;                 "
    ".arch i286             "
    : "=m" (out) 
  );

  return out;
}


float fmul(float a, float b){
  float out;

  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fld  %1;               " // %1 -> st0
    "fld  %2;               " // %2 -> st0, %1 -> st1
    "fmul %%st(1), %%st(0); " // st1 * st0 -> st0
    "fstp %0;               " // st0 -> %0 & pop st0
    "fwait;                 "
    ".arch i286             "
    : "=m" (out) : "m" (a), "m" (b)
  );

  return out;
}

double faddl(double a, double b){
  double out;

  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fldl  %1;              " // %1 -> st0
    "fldl  %2;              " // %2 -> st0, %1 -> st1
    "fadd %%st(1), %%st(0); " // st1 + st0 -> st0
    "fstpl %0;              " // st0 -> %0 & pop st0
    "fwait;                 "
    ".arch i286             "
    : "=m" (out) : "m" (a), "m" (b)
  );

  return out;
}

double fmull(double a, double b){
  double out;

  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fldl  %1;              " // %1 -> st0
    "fldl  %2;              " // %2 -> st0, %1 -> st1
    "fmul %%st(1), %%st(0); " // st1 * st0 -> st0
    "fstpl %0;              " // st0 -> %0 & pop st0
    "fwait;                 "
    ".arch i286             "
    : "=m" (out) : "m" (a), "m" (b)
  );

  return out;
}
