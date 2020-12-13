#include <stdio.h>

int main(void) {
  float a = 1.23;
  float out = 0;

  asm volatile ("finit");
  // asm ("fwait");
  // asm ("fld %0"::"r"(&a));
  // asm ("fstp %0" : "=r"(out) :);


  // asm ("movw %0, %%es:(%%bx)"::"r"(x));

  return 0;
}
