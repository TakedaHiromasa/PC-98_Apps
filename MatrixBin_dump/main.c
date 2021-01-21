#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 512

int main(int argc, char *argv[]){
  int size = 0;

  if(argc > 2){
    size = atoi(argv[2]);
  }else{
    size = SIZE;
  }

  printf("size = %dx%d\n", size, size);

  float *out;
  float *tmp  = malloc(sizeof(float) * size * size);
  float *data = malloc(sizeof(float) * size * size);
  float *dst  = malloc(sizeof(float) * size * size);

  for(long i=0; i<(long)size * size; i++){
    data[i] = i;
  }

  /* 転置行列 */
  if(argc > 3 && strcmp(argv[3], "-T") == 0){
    for(int y=0; y<size; y++){
      for(int x=0; x<size; x++){
        tmp[x*size+y] = data[y*size+x];
      }
    }
    out = tmp;
  }else{
    out = data;
  }

  FILE * fp = NULL;
  if((fp=fopen(argv[1],"wb")) == NULL){
    printf("%sファイルのオープンに失敗\n", argv[1]);
    return -1;
  }
  fwrite(out, sizeof(out[0]), size * size, fp);
  fclose(fp);

  if((fp=fopen(argv[1],"rb")) == NULL){
    printf("%sファイルのオープンに失敗\n", argv[1]);
    return -1;
  }
  fread(dst, sizeof(dst[0]), size * size, fp);
  fclose(fp);

  for(int y=0; y<size; y++){
    for(int x=0; x<size; x++){
      printf("%10.1f ", dst[y*size+x]);
    }
    puts("");
  }
  
}