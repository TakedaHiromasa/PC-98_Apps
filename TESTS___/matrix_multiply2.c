#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __MSDOS__
#include <bios.h>
#include <i86.h>
#include <dos.h>
#include <conio.h>
#endif

#ifdef __MSDOS__
  #define FAR __far
  #define MALLOC(size, ptr) my_malloc(size, &ptr)
#else
  #define FAR 
  #define MALLOC(size, ptr) ({ptr = malloc(size); 0;})
  #define MK_FP(s, o) s
#endif

#ifdef __MSDOS__
int my_malloc(long size, float FAR **ptr){
  unsigned int seg, err;

  if(err = _dos_allocmem(size/16, &seg) != 0)
    return err;

  *ptr = (float FAR *)MK_FP(seg, 0x0000);

  return 0;
}
#endif


#define SIZE 32

int main(int argc, char *argv[]){
  unsigned int size = 0;
  float FAR *a, *b, *c;

  size = (argc > 1) ? atoi(argv[1]) : SIZE;
  printf("[ %dx%d matrix ]\n", size, size);

#ifdef __MSDOS__
  puts("\n=== memory check ===");
  unsigned int seg;
  _dos_allocmem(0xFFFF, &seg);
  long available_mem = (long)seg * 16;
  long data_size     = (long)sizeof(float) * size * size * 3;  // minimum size of overlay heap
  printf("available %lu bytes (%lu KB)\n", available_mem, available_mem/1024);
  printf("data_size %lu bytes (%lu KB)\n", data_size, data_size/1024);

  if(available_mem < data_size){
    puts("\e[31mERROR:Insufficient memory!!\e[m");
    return -1;
  }
#endif

  puts("\n=== alloc ===");
  if(MALLOC((long)sizeof(float)*size*size, a) != 0) return -11;
  if(MALLOC((long)sizeof(float)*size*size, b) != 0) return -12;
  if(MALLOC((long)sizeof(float)*size*size, c) != 0) return -13;
  printf("0x%lx 0x%lx 0x%lx\n", (long)a, (long)b, (long)c);

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