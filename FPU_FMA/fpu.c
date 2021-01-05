#include <stdio.h>
#include <math.h>
#include "../00_include/fpu.h"

void fma_test(){
  float a, b;
  float out;

  puts("** float_test **");
  puts("Real number input:");
  printf("a=");
  scanf("%f", &a);
  printf("b=");
  scanf("%f", &b);

  zeroset();   // zero set 
  fma_r(a,b);  // fma
  fma_r(a,b);  // fma
  out = fpop(); // pop calculation result
  printf("FMA = %f\n", out);

  return;
}

int main(void){

  // FPUを使用する場合
  // 必ず最初にfinitすること！
  finit();

  fma_test();

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
