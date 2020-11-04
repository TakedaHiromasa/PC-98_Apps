#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bios.h>
#include <i86.h>
#include <dos.h>
#include <conio.h>

#define SIZE 32

int main(int argc, char *argv[]){
  unsigned int size = 0;
  unsigned int a_seg, b_seg, c_seg, err;
  float __far *a, *b, *c;

  size = (argc > 1) ? atoi(argv[1]) : SIZE;
  printf("[ %dx%d matrix ]\n", size, size);

  puts("\n=== memory check ===");
  _dos_allocmem(0xFFFF, &a_seg);
  long available_mem = (long)a_seg * 16;
  long data_size     = (long)sizeof(float) * size * size * 3;  // minimum size of overlay heap
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

  a = (float __far *)MK_FP(a_seg, 0x0000);
  b = (float __far *)MK_FP(b_seg, 0x0000);
  c = (float __far *)MK_FP(c_seg, 0x0000);
  printf("0x%lx 0x%lx 0x%lx\n", a, b, c);

  puts("\n=== main ===");
  for(long i=0; i<size*size; i++){
    // printf("0x%lx 0x%lx 0x%lx %d\n", &a[i], &b[i], &c[i], i);
    a[i]=b[i]=i;
    c[i]=0;
  }

  long start_time, end_time;
  // start_time = time(NULL);
  // _bios_timeofday(_TIME_GETCLOCK, &start_time);
  start_time = clock();
  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
      for(long k=0;k<size;k++){
        long a_idx=i*size+k;
        long b_idx=k*size+j;
        long c_idx=i*size+j;
        c[c_idx]+=a[a_idx]*b[b_idx];
      }
    }
  }
  // end_time = time(NULL);
  end_time = clock();
  // _bios_timeofday(_TIME_GETCLOCK, &end_time);
  printf("c[0]=%f\n", c[0]);
  printf("start_time = %lu\n", start_time);
  printf("end_time   = %lu\n", end_time);
  printf("time = %lu\n", end_time - start_time);

  // print calculation result 
  /* for(int i=0;i<size;i++){ */
  /*   for(int j=0;j<size;j++){ */
  /*     printf("%f,",c[i*size+j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */
}