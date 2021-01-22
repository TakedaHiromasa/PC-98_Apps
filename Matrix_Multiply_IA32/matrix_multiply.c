#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 512

double getCurtMsec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1e+3 + (double)tv.tv_usec*1e-3;
}

int main(){
  float *a = malloc(sizeof(float)*SIZE*SIZE);
  float *b = malloc(sizeof(float)*SIZE*SIZE);
  float *c = malloc(sizeof(float)*SIZE*SIZE);

  for(long i=0; i<(long)SIZE*SIZE; i++){
    a[i] = b[i] = i/1000.0;
    c[i] = 0;
  }
  
  double start_time, end_time;
  start_time = getCurtMsec();
  for(long i=0; i<SIZE; i++){
    for(long j=0; j<SIZE; j++){
      for(long k=0; k<SIZE; k++){
        long a_idx = i * SIZE + k;
        long b_idx = k * SIZE + j;
        long c_idx = i * SIZE + j;
        c[c_idx] += a[a_idx] * b[b_idx];
      }
    }
  }
  end_time = getCurtMsec();

  printf("c[0] = %f\n", c[0]);
  printf("c[%d] = %f\n", (SIZE*SIZE)-1, c[(SIZE*SIZE)-1]);
  printf("time: %.5f[msec]\n", end_time - start_time);

  // print calculation result 
  // for(int i=0;i<SIZE;i++){
  //   for(int j=0;j<SIZE;j++){
  //     printf("%15.5f,",c[i*SIZE+j]);
  //   }
  //   printf("\n");
  // }
}