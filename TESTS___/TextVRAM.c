#include <stdio.h>

#define MK_FP(seg, off) (((long)seg << 0x10) | (long)off)

int main(void) {
  printf("Hello TEXT!\n");

  wchar_t __far *a;
  // a = (wchar_t __far *)MK_FP(0xa000, 0x0000);  // 0xa0000000L;
  a = (wchar_t __far *)0xa0000000L;  // 0xa0000000L;

  printf("addr = 0x%lx\n", a);

  a += (80 * 10);  // 10行目から

  // 1面=80x25
  for(int i=0; i<80; i++){
    *a = (wchar_t)0x41;
    a++;
  }

  return 0;
}


/* // ASM /////
asm ("movw $0xa000, %ax");
asm ("movw %ax, %es");
asm ("movw $0x0000, %bx");
asm ("movw $0x41, %es:(%bx)");
//////////// */