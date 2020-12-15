#include <stdio.h>
#include <math.h>

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

int main(void){
  double a, b;
  double out;

  puts("Real number input:");
  printf("a=");
  scanf("%lf", &a);
  printf("b=");
  scanf("%lf", &b);

  out = faddl(a, b);

  printf("FADD = %f\n", out);


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