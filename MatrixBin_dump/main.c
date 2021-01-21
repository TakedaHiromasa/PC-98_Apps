#include <stdio.h>
#include <stdlib.h>

#define SIZE 512

int main(int argc, char *argv[]){
  int size = 0;

  if(argc > 2){
    size = atoi(argv[2]);
  }else{
    size = SIZE;
  }

  printf("size = %dx%d\n", size, size);

  float *data = malloc(sizeof(float) * size * size);
  float *dst  = malloc(sizeof(float) * size * size);

  for(long i=0; i<(long)size * size; i++){
    data[i] = i;
  }

  FILE * fp = NULL;
  if((fp=fopen(argv[1],"wb")) == NULL){
    printf("%sファイルのオープンに失敗\n", argv[1]);
    return -1;
  }
  fwrite(data, sizeof(data[0]), size * size, fp);
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