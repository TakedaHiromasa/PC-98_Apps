#include <stdio.h>

int main(void) {
  printf("Hello VRAM!\n");

  int16_t x = 0xff;

  // グラフィック使用開始
  asm ("mov $0x40, %ah");
  asm ("int $0x18");

  asm ("movw $0xa800, %ax");
  asm ("movw %ax, %es");
  for(int i=0; i<0x8000; i++){
    asm ("movw %0, %%bx"::"r"(0x0000+i));
    asm ("movw %0, %%es:(%%bx)"::"r"(x));
  }

  return 0;
}