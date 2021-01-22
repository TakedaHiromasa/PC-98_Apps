#include <stdio.h>
#include <stdlib.h>
#include <i86.h>
#include "xms.h"

char XMS_exists(){
  char err_code = 1;
  __asm volatile(
    "CHK_XMSDRV:;"
    "mov $0x4300, %%ax;"
    "int $0x2F;"
    "cmp $0x80, %%al;"
    "jz  CHK_XMSDRV_OK;"
    "mov $0x01, %0;"
    "jmp END;"
    
    "CHK_XMSDRV_OK:;"
    "mov $0x00, %0;"
    
    "END:;"
  : "=r"(err_code) :);

  return err_code;
}

void __far *XMS_init(){
  unsigned int seg, off;
  void __far *xms_dep = NULL;

  __asm volatile(
    "mov $0x4310, %%ax;"
    "int $0x2F;"
    "mov %%bx, %0;"
    "mov %%es, %1;"
    "mov %%bx, (XMS_CALL+0);"
    "mov %%es, (XMS_CALL+2);"
    "XMS_CALL:;"
  : "=r"(off), "=r"(seg) :);

  xms_dep = (void __far *)MK_FP(seg, off);

  return xms_dep;
}

void XMS_getinfo(XMS_INFO *info){
  info->var = 0;
  info->rev = 0;
  info->hma = 0;
  info->var_txt = NULL;
  info->var_txt = malloc(8 * sizeof(char));

  __asm volatile(
    "mov   $0x0, %%ah;"
    "lcall *XMS_CALL;"
    "movw  %%ax, %0;"
    "movw  %%bx, %1;"
    "movw  %%dx, %2;"
    : "=r"(info->var), "=r"(info->rev), "=r"(info->hma) :);
  sprintf(info->var_txt, "%2d.%02d", (info->var >> 8), (info->var & 0xFF));

  return;
}

char XMS_getFreeSpace(unsigned int *max_block, unsigned int *total){
  char err_code;

  *max_block = 0;
  *total     = 0;

  __asm volatile(
  "mov   $0x08, %%ah;"
  "lcall *XMS_CALL;"
  "movw  %%ax, %0;"
  "movw  %%dx, %1;"
  "mov   %%bl, %2;"
  : "=r"(*max_block), "=r"(*total), "=r"(err_code) :);

  if((*max_block|*total) != 0) err_code = 0;

  return err_code;
}

char XMS_malloc(SMEM *mem_p, unsigned long size){
  unsigned int size_KB  = 0;
  unsigned int handle   = 0;
  int  success  = 0;
  char err_code = 0;

  size_KB = size / 1024 + 1;
  __asm volatile(
  "movw  %3,    %%dx;"
  "mov   $0x09, %%ah;"
  "lcall *XMS_CALL;  "
  "movw  %%ax,  %0;  "
  "mov   %%bl,  %1;  "
  "movw  %%dx,  %2;  "
  : "=r"(success), "=r"(err_code), "=r"(handle) : "r"(size_KB) : "%dx");
  mem_p->handle = handle;
  mem_p->linptr = 0L;

  if(success) err_code = 0;

  return err_code;
}

int XMS_free(SMEM *mem_p){
  int  success  = 0;
  char err_code = 0;

  __asm volatile(
  "movw  %2,    %%dx;"
  "mov   $0x0a, %%ah;"
  "lcall *XMS_CALL;  "
  "movw  %%ax,  %0;  "
  "mov   %%bl,  %1;  "
  : "=r"(success), "=r"(err_code) : "r"(mem_p->handle) : "%dx");

  if(success) err_code = 0;

  return err_code;
}

unsigned long XMS_write(SMEM *mem_p, void __far *buf, unsigned long size){
  char success  = 0;
  char err_code = 0;

  XMS_MVPARAM param;
  param.Length_B     = size;
  param.SourceHandle = 0;
  param.SourceOffset = (unsigned long)buf;
  param.DestHandle   = (unsigned int)mem_p->handle;
  param.DestOffset   = mem_p->linptr;

  __asm volatile(
  "movw  %2,    %%si;"
  "mov   $0x0b, %%ah;"
  "lcall *XMS_CALL;  "
  "mov   %%ah,  %0;  "
  "mov   %%bl,  %1;  "
  : "=r"(success), "=r"(err_code) : "r"(FP_OFF(&param)) : "%si");

  if(!success){
    return (unsigned long)err_code;
  }
  mem_p->linptr += size;
  return size;
}

unsigned long XMS_read(SMEM *mem_p, void __far *buf, unsigned long size){
  char success  = 0;
  char err_code = 0;

  XMS_MVPARAM param;
  param.Length_B     = size;
  param.SourceHandle = (unsigned int)mem_p->handle;
  param.SourceOffset = mem_p->linptr;
  param.DestHandle   = 0;
  param.DestOffset   = (unsigned long)buf;

  __asm volatile(
  "movw  %2,    %%si;"
  "mov   $0x0b, %%ah;"
  "lcall *XMS_CALL;  "
  "mov   %%ah,  %0;  "
  "mov   %%bl,  %1;  "
  : "=r"(success), "=r"(err_code) : "r"(FP_OFF(&param)) : "%si");

  if(!success){
    return (unsigned long)err_code;
  }
  // mem_p->linptr += size;
  return size;
}