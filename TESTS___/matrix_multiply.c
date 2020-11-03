#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 32
int main(){
  float *a=malloc(sizeof(float)*SIZE*SIZE);
  float *b=malloc(sizeof(float)*SIZE*SIZE);
  float *c=malloc(sizeof(float)*SIZE*SIZE);
  for(long i=0;i<(long)SIZE*SIZE;i++){
    a[i]=b[i]=i;
    c[i]=0;
  }
  //time_t start_time,end_time;
  long start_time,end_time;
  //start_time=time(NULL);
  start_time=clock();
  for(long i=0;i<SIZE;i++){
    for(long j=0;j<SIZE;j++){
      for(long k=0;k<SIZE;k++){
	long a_idx=i*SIZE+k;
	long b_idx=k*SIZE+j;
	long c_idx=i*SIZE+j;
	c[c_idx]+=a[a_idx]*b[b_idx];
      }
    }
  }
  //end_time=time(NULL);
  end_time=clock();
  printf("c[0]=%f\n",c[0]);
  printf("%d,%d\n",start_time,end_time);

  // print calculation result 
  /* for(int i=0;i<SIZE;i++){ */
  /*   for(int j=0;j<SIZE;j++){ */
  /*     printf("%f,",c[i*SIZE+j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */
}
