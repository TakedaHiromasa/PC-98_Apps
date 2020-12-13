#include <stdio.h>
#include <i86.h>
#include "../00_include/dos_timer.h"

int main(void){
  TIME start_time, end_time;

  printf("Hello Timer!\n");
  printf("Type key [Enter] = START & STOP\n");

  while(getchar() != '\n');
  get_time(&start_time);
  puts("->TIMER START!");
  print_time(&start_time);

  while(getchar() != '\n');
  get_time(&end_time);
  puts("->TIMER STOP!");
  print_time(&end_time);

  print_difftime(&start_time, &end_time);

  return 0;
}