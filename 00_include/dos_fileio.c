#ifdef __MSDOS__

#include <stdio.h>
#include "dos_fileio.h"

typedef unsigned int uint;

int dos_fopen(const char* file_name){
  int handle = 0;
  uint err_code = 0;

  __asm volatile(
    "mov $0x3D, %%ah;"
    "mov $0x00, %%al;"
    // "movw %, %%ds;"
    "movw %2, %%dx;"
    "int $0x21;"
    "jc  ERROR;"
    "movw %%ax, %0;"
    "movw $0,   %1;"
    "jmp END;"
    "ERROR:    ;"
    "movw $-1,  %0;"
    "movw %%ax, %1;"
    "END:      ;"
    ""
  : "=bx"(handle), "=r"(err_code) : "m"(file_name) : "%%ds", "%%dx", "%%bx"
  );

  if(handle == -1){
    // ERROR_CODE:
    // AX = 0001 : シェアリングモードが無効
    // AX = 0002 : ファイル名が無効またはファイルが存在しない
    // AX = 0003 : パス名が無効または存在しない
    // AX = 0004 : オープンしているファイル数が多すぎる
    // AX = 0005 : ディレクトリやボリュームをオープンしようとした。またはリードオンリーファイルをライトモードでオープンしようとした
    // AX = 000C : アクセスモードが無効
    return (err_code * -1);
  }

  return handle;
}

int dos_fclose(uint handle){
  uint err_code = 0;

  __asm volatile(
    "mov  $0x3E, %%ah;"
    "movw %1, %%bx;  "
    "int $0x21;      "
    "jc  ERROR2;      "
    "movw $0,  %0;    "
    "jmp END2;        "
    "ERROR2: ;        "
    "movw %%ax, %0;   "
    "END2: ;          "
    ""
  : "=r"(err_code) : "bx"(handle) : "%%bx"
  );

  // ERROR_CODE:
  // AX = 0006 : ファイルハンドルが無効
  return (err_code * -1);
}

int dos_fread(void __far *array, size_t size, size_t n, uint handle){
  uint err_code = 0;
  
  int loop_n = (size*n)/BUFFSIZE;
  int edge = (size*n)%BUFFSIZE;

  if((size*n) < BUFFSIZE){
    __asm volatile(
      "movw %0, %%bx;  "
      "movw %1, %%cx;  "
      "lds  %2, %%dx;"
      "mov  $0x3f, %%ah;"
      "int  $0x21;     "
      ""
    : : "bx"(handle), "cx"(size*n), "m"(array) : "%%bx", "%%cx", "%%ds", "%%dx"
    );
    
    return 0;
  }

  __asm volatile(
    "movw %0, %%bx;  "
    "movw %1, %%cx;  "
    "lds  %2, %%dx;"
    ""
  : : "bx"(handle), "cx"(BUFFSIZE), "m"(array) : "%%bx", "%%cx", "%%ds", "%%dx"
  );

  for (int i = 0; i < loop_n; i++){
    __asm volatile(
      "mov  $0x3f, %%ah;"
      "int  $0x21;     "
      "add  %%cx, %%dx;"
      : : :
    );
  }

  __asm volatile(
    "movw %1,    %%cx;"
    "mov  $0x3f, %%ah;"
    "int  $0x21;     "
    ""
  : : "bx"(handle), "cx"(edge) : "%%cx"
  );

  return 0;
}
#endif