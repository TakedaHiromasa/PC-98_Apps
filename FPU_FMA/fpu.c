#include <stdio.h>
#include <math.h>
#include "../00_include/fpu.h"

void float_test(){
  float a, b;
  float out;

  puts("** float_test **");
  puts("Real number input:");
  printf("a=");
  scanf("%f", &a);
  printf("b=");
  scanf("%f", &b);
  zeroset(); // zero set 
  fma_r(a,b); // fma
  fma_r(a,b); // fma
  out=pop();  // pop calculation result
  printf("FMA = %f\n", out);
  /* out = fadd(a, b); */
  /* printf("FADD = %f\n", out); */

  /* out = fmul(a, b); */
  /* printf("FMUL = %f\n", out); */

  return;
}

void double_test(){
  double a, b;
  double out;

  puts("** double_test **");
  puts("Real number input:");
  printf("a=");
  scanf("%lf", &a);
  printf("b=");
  scanf("%lf", &b);

  out = faddl(a, b);
  printf("FADDL = %f\n", out);

  out = fmull(a, b);
  printf("FMULL = %f\n", out);

  return;
}

int main(void){

  finit();

  float_test();
  // double_test();

  // LD-ST test
  int i = 1;
  double out;
  __asm volatile(
    ".arch pentium; "
    "finit; "
    "fild  %1; "
    "fstpl %0; "
    "fwait; "
    ".arch i286"
    : "=m" (out) : "m" (i)
  );

  printf("LD_ST = %f\n", out);

  return 0;
}