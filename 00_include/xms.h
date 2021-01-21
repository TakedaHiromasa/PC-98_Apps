#ifndef XMS_H
#define XMS_H

// 成功は０で定義します
#define E_SUCCESS          (0x00)
// エラー判定用のマクロ
#define E_IS_SUCCESS(err)  ((err) == E_SUCCESS)
#define E_HAS_ERROR(err)   ((err) != E_SUCCESS)

typedef struct {
  unsigned long  Length_B;      /* 転送するバイト数   */
  unsigned int   SourceHandle;  /* 転送元のハンドル   */
  unsigned long  SourceOffset;  /* 転送元のオフセット */
  unsigned int   DestHandle;    /* 転送先のハンドル   */
  unsigned long  DestOffset;    /* 転送先のオフセット */
} XMS_MVPARAM;

typedef struct {
  unsigned int var;
  unsigned int rev;
  unsigned int hma;
  char *var_txt;
} XMS_INFO;

typedef struct {
  unsigned int   handle;  /* ハンドル       */
  unsigned long  linptr;  /* リニア・ポインタ */
} SMEM;

char XMS_exists();
void __far *XMS_init();
void XMS_getinfo(XMS_INFO *info);
char XMS_getFreeSpace(unsigned int *max_block, unsigned int *total);
char XMS_malloc(SMEM *mem_p, unsigned long size);
int  XMS_free(SMEM *mem_p);
unsigned long XMS_write(SMEM *mem_p, void __far *buf, unsigned long size);
unsigned long XMS_read(SMEM *mem_p, void __far *buf, unsigned long size);

#endif