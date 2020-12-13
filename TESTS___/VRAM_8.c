#include <stdio.h>
#include <i86.h>

int main(void) {
  printf("Hello VRAM!\n");

  char __far *vram_P0_addr;
  char __far *vram_P1_addr;
  char __far *vram_P2_addr;
  vram_P0_addr = (char __far *)MK_FP(0xa800, 0x0000);
  vram_P1_addr = (char __far *)MK_FP(0xb000, 0x0000);
  vram_P2_addr = (char __far *)MK_FP(0xb800, 0x0000);

  // グラフィック設定
  asm ("mov $0x42, %ah");
  asm ("mov $0b11000000, %ch");
  asm ("int $0x18");

  // グラフィック使用開始
  asm ("mov $0x40, %ah");
  asm ("int $0x18");

  // for(int i=0; i<400*(640/8); i++){
  //   *vram0_addr = 0xff;
  //   vram0_addr++;
  // }

  for(int y=0; y<400; y++){
    for(int x=0; x<(640/8); x++){
      vram_P0_addr[y*80+x] = 0xff;
      vram_P1_addr[y*80+x] = 0x00;
      vram_P2_addr[y*80+x] = 0x00;
    }
  }

  return 0;
}