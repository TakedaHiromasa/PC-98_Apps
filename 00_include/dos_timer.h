#ifndef DOS_TIMER_H
#define DOS_TIMER_H

typedef struct _TIME {
  char hr;
  char min;
  char sec;
  char subs;
} TIME;

void get_time(TIME *time_ptr);
void print_difftime(TIME *start_time_ptr, TIME *end_time_ptr);
void print_time(TIME *time_ptr);

#endif