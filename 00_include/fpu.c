#include "fpu.h"

void finit(){
  __asm volatile(
    ".arch pentium;         "
    "finit;                 "
    "fwait;                 "
    ".arch i286             "
  );
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
    "fmul %%st(1), %%st(0); " // st1 + st0 -> st0
    "fstpl %0;              " // st0 -> %0 & pop st0
    "fwait;                 "
    ".arch i286             "
    : "=m" (out) : "m" (a), "m" (b)
  );

  return out;
}