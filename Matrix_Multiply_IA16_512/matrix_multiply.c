#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <i86.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include "../00_include/fpu.h"
#include "../00_include/dos_fileio.h"
#include "../00_include/dos_timer.h"

#define SIZE 512

void put_vram(char __far *addr, int box_size, int off_y, int off_x, int y, int x, char val){
  unsigned int y_point = off_y + (y * box_size);
  unsigned int x_point = off_x + (x * box_size);

  for(int box_y=0; box_y<box_size; box_y++){
    for(int box_x=0; box_x<box_size; box_x++){
      addr[(((y_point+box_y)*80)+((x_point+box_x)/8))] = val;
    }
  }
}

// int main(int argc, char *argv[]){
//   unsigned int a_seg, b_seg, c_seg, err;
//   float __far *a, *b, *c;

//   puts("\n=== alloc ===");
//   if(err = _dos_allocmem((long)sizeof(float) * 512 /16 + 1, &a_seg) != 0) return -11;
//   if(err = _dos_allocmem((long)sizeof(float) * 512 /16 + 1, &b_seg) != 0) return -12;
//   if(err = _dos_allocmem((long)sizeof(float) * 512 /16 + 1, &c_seg) != 0) return -13;

//   a = (float __far *)MK_FP(a_seg, 0x0000);
//   b = (float __far *)MK_FP(b_seg, 0x0000);
//   c = (float __far *)MK_FP(c_seg, 0x0000);

//   uint Abin_handle = 0;
//   Abin_handle = dos_fopen("../MM512BIN/B_T.bin");
//   for (int i = 0; i < 2; i++){
//     dos_fread((void __far *)a, sizeof(float), 512, Abin_handle); // 値を512個(1行分)読み込む
    
//     for(int x=0; x<512; x++){
//       printf("%5.1f ", a[x]);
//     }
//     puts("");
//   }
//   dos_fclose(Abin_handle);
// }

int main(int argc, char *argv[]){
  unsigned int size = 0;
  unsigned int a_seg, b_seg, c_seg,cc_seg, err;
  float __far *a, *b, *c, *cc;

  size = (argc > 1) ? atoi(argv[1]) : SIZE;
  printf("[ %dx%d matrix ]\n", size, size);

  puts("\n=== memory check ===");
  _dos_allocmem(0xFFFF, &a_seg);
  long available_mem = (long)a_seg * 16;
  long data_size     = (long)sizeof(float) * size * size * 4;  // minimum size of overlay heap
  printf("available %lu bytes (%lu KB)\n", available_mem, available_mem/1024);
  printf("data_size %lu bytes (%lu KB)\n", data_size, data_size/1024);

  if(available_mem < size){
    puts("\e[31mERROR:Insufficient memory!!\e[m");
    return -1;
  }

  puts("\n=== alloc ===");
  if(err = _dos_allocmem((long)sizeof(float) * size/16, &a_seg) != 0) return -11;
  if(err = _dos_allocmem((long)sizeof(float) * size/16, &b_seg) != 0) return -12;
  if(err = _dos_allocmem((long)sizeof(float) * size/16, &c_seg) != 0) return -13;
  if(err = _dos_allocmem((long)sizeof(float) * size/16, &cc_seg) != 0) return -14;

  a  = (float __far *)MK_FP(a_seg, 0x0000);
  b  = (float __far *)MK_FP(b_seg, 0x0000);
  c  = (float __far *)MK_FP(c_seg, 0x0000);
  cc = (float __far *)MK_FP(cc_seg, 0x0000);
  printf("0x%lx 0x%lx 0x%lx\n", a, b, c);

  puts("\n=== main ===");
  printf("Running"); fflush(stdout);
  char __far *vram0_addr;
  char __far *vram1_addr;
  vram0_addr = (char __far *)MK_FP(0xa800, 0x0000);
  vram1_addr = (char __far *)MK_FP(0xb000, 0x0000);
  
  // グラフィック使用開始
  asm ("mov $0x40, %ah");
  asm ("int $0x18");
  for(long i=0;i<(size/4+8);i++){
    for(long j=0;j<(size/4+2);j++){
        put_vram(vram1_addr, 2, 10-8, 320-1, i, j, 0xff);
    }
  }

  for(long i=0;i<size/4;i++){
    for(long j=0;j<size/4;j++){
        put_vram(vram1_addr, 2, 10, 320, i, j, 0x00);
    }
  }


  printf(" -> Matrix multiply\n"); fflush(stdout);
  TIME start_time, end_time, seed_time;
  uint Abin_handle = dos_fopen("../MM512BIN/A.bin");
  uint Cbin_handle = dos_fopen("C.bin");
  finit();

  get_time(&start_time);
  for(long i=0;i<size;i++){
    dos_fread((void __far *)a, sizeof(float), size, Abin_handle); // 値を512個(1行分)読み込む
    uint Bbin_handle = dos_fopen("../MM512BIN/B_T.bin");
    for(long j=0;j<size;j++){
      dos_fread((void __far *)b, sizeof(float), size, Bbin_handle); // 値を512個(1行分)読み込む
      finit();
      zeroset(); // zero set
      long c_idx = j; //i*size+j
      c[c_idx] = 0.0;
      for(long k=0; k<size; k++){
        long a_idx = k; //i*size+k
        long b_idx = k; //k*size+j
        // c[c_idx] = fadd(c[c_idx], fmul(a[a_idx], b[b_idx])); // こちらの方が誤差が小さい
        fma_r(a[a_idx], b[b_idx]);
      }
      c[c_idx] = fpop();
      put_vram(vram0_addr, 2, 10, 320, i/4, j/4, 0xff);
    }
    dos_fclose(Bbin_handle);

    dos_fwrite((void __far *)c, sizeof(float), size, Cbin_handle);
  }
  get_time(&end_time);

  dos_fclose(Abin_handle);
  dos_fclose(Cbin_handle);

  // printf("c[0] = %f\n", c[0]);
  printf("start_");
  print_time(&start_time);
  printf("  end_");
  print_time(&end_time);
  print_difftime(&start_time, &end_time);

  for(int i=0; i<atoi(argv[2]); i++){
    printf("\a");
  }
}
