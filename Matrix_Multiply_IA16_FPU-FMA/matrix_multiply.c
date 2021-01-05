#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <i86.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include "../00_include/fpu.h"

#include "../00_include/dos_timer.h"

#define SIZE 32

void put_vram(char __far *addr, int box_size, int off_y, int off_x, int y, int x, char val){
  unsigned int y_point = off_y + (y * box_size);
  unsigned int x_point = off_x + (x * box_size);

  for(int box_y=0; box_y<box_size; box_y++){
    for(int box_x=0; box_x<box_size; box_x++){
      addr[(((y_point+box_y)*80)+((x_point+box_x)/8))] = val;
    }
  }
}

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

  if(available_mem < data_size){
    puts("\e[31mERROR:Insufficient memory!!\e[m");
    return -1;
  }

  puts("\n=== alloc ===");
  if(err = _dos_allocmem((long)sizeof(float)*size*size/16, &a_seg) != 0) return -11;
  if(err = _dos_allocmem((long)sizeof(float)*size*size/16, &b_seg) != 0) return -12;
  if(err = _dos_allocmem((long)sizeof(float)*size*size/16, &c_seg) != 0) return -13;
  if(err = _dos_allocmem((long)sizeof(float)*size*size/16, &cc_seg) != 0) return -14;

  a = (float __far *)MK_FP(a_seg, 0x0000);
  b = (float __far *)MK_FP(b_seg, 0x0000);
  c = (float __far *)MK_FP(c_seg, 0x0000);
  cc=(float __far *)MK_FP(cc_seg, 0x0000);
  printf("0x%lx 0x%lx 0x%lx\n", a, b, c);

  puts("\n=== main ===");
  puts("Running...");
  char __far *vram0_addr;
  char __far *vram1_addr;
  vram0_addr = (char __far *)MK_FP(0xa800, 0x0000);
  vram1_addr = (char __far *)MK_FP(0xb000, 0x0000);
  
  // グラフィック使用開始
  asm ("mov $0x40, %ah");
  asm ("int $0x18");
  for(long i=0;i<size+8;i++){
    for(long j=0;j<size+2;j++){
        put_vram(vram1_addr, 2, 10-8, 320-1, i, j, 0xff);
    }
  }

  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
        put_vram(vram1_addr, 2, 10, 320, i, j, 0x00);
    }
  }

  for(long i=0; i<size*size; i++){
    // printf("0x%lx 0x%lx 0x%lx %d\n", &a[i], &b[i], &c[i], i);
    a[i]=b[i]=i;
    c[i]=cc[i]=0;
  }

  TIME start_time, end_time;
  get_time(&start_time);
  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
      zeroset(); // zero set
      long c_idx=i*size+j;
      for(long k=0;k<size;k++){
        long a_idx=i*size+k;
        long b_idx=k*size+j;
        //c[c_idx]+=a[a_idx]*b[b_idx];
        //c[c_idx]=fadd(c[c_idx],fmul(a[a_idx],b[b_idx]));
	fma_r(a[a_idx],b[b_idx]);
      }
      c[c_idx]=pop();
      //put_vram(vram0_addr, 2, 10, 320, i, j, 0xff);
    }
  }
  get_time(&end_time);
  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
      for(long k=0;k<size;k++){
        long a_idx=i*size+k;
        long b_idx=k*size+j;
        long c_idx=i*size+j;
        cc[c_idx]+=a[a_idx]*b[b_idx];
      }
    }
  }

  float ave=0;
  float min=1000000;
  float max=-1000000;
  float lmin=1000000;
  float lmax=-1000000;
  float err_sum=0;
  for(int i=0;i<size*size;i++){
    min = fminf(fabs(c[i]-cc[i]),min);
    max = fmaxf(fabs(c[i]-cc[i]),max);
    lmin = fminf(fabs(c[i]-cc[i])/cc[i],lmin);
    lmax = fmaxf(fabs(c[i]-cc[i])/cc[i],lmax);
    err_sum+=fabs(c[i]-cc[i]);
    ave+=cc[i];
  }
  err_sum=(float)err_sum/(size*size);
  ave=ave/(size*size);
  printf("absolute min error:%le\n",min);
  printf("absolute max error:%le\n",max);
  printf("relative min error:%le\n",lmin);
  printf("relative max error:%le\n",lmax);
  printf("error average:%le\n",err_sum);
  printf("average:%le\n",ave);

  
  printf("c[0]=%f\n", c[0]);
  printf("cc[0]=%f\n", cc[0]);
  printf("start_");
  print_time(&start_time);
  printf("  end_");
  print_time(&end_time);
  print_difftime(&start_time, &end_time);

  for(int i=0; i<atoi(argv[2]); i++){
    printf("\a");
  }

  // print calculation result 
  /* for(int i=0;i<size;i++){ */
  /*   for(int j=0;j<size;j++){ */
  /*     printf("%f,",c[i*size+j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */
}
