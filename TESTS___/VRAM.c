#include <stdio.h>

#define FP(seg, off) (((long)seg << 0x10) | (long)off)

int main(void) {
  printf("Hello VRAM!\n");

  char __far *vram0_addr;
  vram0_addr = (char __far *)FP(0xa800, 0x0000);

  // グラフィック使用開始
  asm ("mov $0x40, %ah");
  asm ("int $0x18");

  for(int i=0; i<0x8000; i++){
    *vram0_addr = 0xff;
    vram0_addr++;
  }

  return 0;
}