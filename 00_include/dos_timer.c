#include <stdio.h>
#include <i86.h>
#include "dos_timer.h"

void get_time(TIME *time_ptr){
  // 時刻の読み出し
  asm ("mov $0x2c, %ah");
  asm ("int $0x21");

  asm ("movb %%ch, %0" : "=r"(time_ptr->hr) :);
  asm ("movb %%cl, %0" : "=r"(time_ptr->min) :);
  asm ("movb %%dh, %0" : "=r"(time_ptr->sec) :);
  asm ("movb %%dl, %0" : "=r"(time_ptr->subs) :);
}

void print_difftime(TIME *start_time_ptr, TIME *end_time_ptr){
  printf("diff_time: %02d:%02d:%02d.%02d\n", 
        (start_time_ptr->hr   - end_time_ptr->hr  ), 
        (start_time_ptr->min  - end_time_ptr->min ), 
        (start_time_ptr->sec  - end_time_ptr->sec ), 
        (start_time_ptr->subs - end_time_ptr->subs));
}

void print_time(TIME *time_ptr){
  printf("time: %02d:%02d:%02d.%02d\n", time_ptr->hr, time_ptr->min, time_ptr->sec, time_ptr->subs);
}