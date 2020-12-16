#include <stdio.h>
#include <math.h>
#include "../00_include/fpu.h"

int main(void){
  double a, b;
  double out;

  puts("Real number input:");
  printf("a=");
  scanf("%lf", &a);
  printf("b=");
  scanf("%lf", &b);

  finit();

  out = faddl(a, b);
  printf("FADD = %f\n", out);

  out = fmull(a, b);
  printf("FMUL = %f\n", out);


  // LD-ST test
  int i = 1;
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