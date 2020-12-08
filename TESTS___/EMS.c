/*---------------------------------------------------------------------------
 *
 *   EMS (FGALEL MES2 Sample)  1995/03/08
 *   by Sey_ju_row (QGA02240@niftyserve.or.jp)
 *
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <fcntl.h>

typedef unsigned long ulong;
typedef unsigned int  uint;
typedef unsigned char uchar;

#ifdef  FP_OFF
#undef  FP_OFF
#define FP_OFF(p) ((uint)(void __far*)p)
#endif
#ifdef  FP_SEG
#undef  FP_SEG
#define FP_SEG(p) ((uint)(((ulong)((void __far*)p))>>16))
#endif

union  REGS   regs; /* INT用バッファ */
struct SREGS sregs; /* INT用バッファ */

#define EMSPUT    1
#define EMSGET    2

#define setseg(ptr,seg,off) \
    segread(&sregs),\
    sregs.seg  = FP_SEG(ptr),\
    regs.x.off = FP_OFF(ptr)

typedef struct {
  ulong MemLength;     /* 転送するバイト数                     */
  uchar SourceType;    /* 転送元のメモリ・タイプ（0:標準 1:拡張） */
  uint  SourceHandle;  /* 転送元のハンドル（0:標準メモリ）       */
  uint  SourceOffset;  /* 転送元のオフセット                   */
  uint  SourceSegPage; /* 転送元のセグメントまたは論理ページ番号   */
  uchar DestType;      /* 転送先のメモリ・タイプ（0:標準 1:拡張） */
  uint  DestHandle;    /* 転送先のハンドル（0:標準メモリ）       */
  uint  DestOffset;    /* 転送先のオフセット                   */
  uint  DestSegPage;   /* 転送先のセグメントまたは論理ページ番号  */
} stEMSMemMov;

typedef struct {
  uint  handle;    /* ハンドル           */
  ulong linptr;   /* リニア・ポインタ     */
  uint  log_pages; /* 論理ﾍﾟｰｼﾞの数      */
} EMEM;

int ems_frame;   /* EMS ﾍﾟｰｼﾞ･ﾌﾚｰﾑ･ｾｸﾞﾒﾝﾄ  */
int free_pages;  /* 空き論理ﾍﾟｰｼﾞの数    */

int EMSinit(void){
  int fp;

  if (( fp = open("EMMXXXX0", O_RDONLY )) != -1 ) {
    close ( fp );
    regs.h.ah = 0x40;        /* EMS ｽﾃｰﾀｽの取得         */
    int86 ( 0x67, &regs, &regs );
    if ( regs.h.ah ) return( -1 );
    regs.h.ah = 0x41;        /* ﾍﾟｰｼﾞ･ﾌﾚｰﾑのｾｸﾞﾒﾝﾄ･ｱﾄﾞﾚｽ取得 */
    int86 ( 0x67, &regs, &regs );
    ems_frame = regs.x.bx;
    regs.h.ah = 0x42;        /* 未ｱﾛｹｰﾄ･ﾍﾟｰｼﾞ数の取得     */
    int86 ( 0x67, &regs, &regs );
    free_pages = regs.x.bx;
    if ( regs.h.ah == 0 ) return( 0 );
  }
  return( -1 );
}

int EMSalloc( ulong size, EMEM *pmem ){
  uint  pages;

  pages = size / 0x4000 + 1;    /* 確保する論理ﾍﾟｰｼﾞの数    */
  if ( free_pages >= pages ) {
    regs.x.bx = pages;
    regs.h.ah = 0x43;           /* 論理ﾍﾟｰｼﾞの確保        */
    int86 ( 0x67, &regs, &regs );
    if ( regs.h.ah != 0 ) return( -1 );
    pmem->handle  = regs.x.dx;  /* ハンドル            */
    pmem->log_pages  = pages;   /* 確保された論理ﾍﾟｰｼﾞの数    */
    return( 0 );
  }
  return( -1 );
}

void __far *EMSallocmem(EMEM *pmem){
  uint  i;

  if ( pmem->log_pages <= 4 ) {
    for ( i = 0; pmem->log_pages > i; i++ ) {
      regs.h.al = i;        /* 使用する物理ﾍﾟｰｼﾞNo.     */
      regs.x.bx = i;        /* 割当てる論理ﾍﾟｰｼﾞNo.     */
      regs.h.ah = 0x44;     /* 論理ﾍﾟｰｼﾞの物理ﾍﾟｰｼﾞへの割当 */
      int86 ( 0x67, &regs, &regs );
    };
    return( MK_FP( ems_frame, 0 ));
  }
  return( NULL );
}

uint EMSfree(uint handle){
  regs.x.dx = handle;
  regs.h.ah = 0x45;      /* EMS ﾒﾓﾘ解放 */
  int86(0x67,&regs,&regs);
  if ( regs.h.ah ) return( -1 );
  return( 0 );
}

ulong EMSMemMov(int dir, EMEM *pmem, void *buf, ulong size){
  uint  page, offset;
  stEMSMemMov semm;

  page   = pmem->linptr / 0x4000;
  offset = pmem->linptr % 0x4000;
  if ( dir == EMSPUT ) {
    semm.MemLength     = size;
    semm.SourceType     = 0;
    semm.SourceHandle  = 0;
    semm.SourceOffset  = FP_OFF ( buf );
    semm.SourceSegPage = FP_SEG ( buf );
    semm.DestType      = 1;
    semm.DestHandle    = pmem->handle;
    semm.DestOffset     = offset;
    semm.DestSegPage   = page;
  } else if ( dir == EMSGET ) {
    semm.MemLength     = size;
    semm.SourceType     = 1;
    semm.SourceHandle  = pmem->handle;
    semm.SourceOffset  = offset;
    semm.SourceSegPage = page;
    semm.DestType     = 0;
    semm.DestHandle     = 0;
    semm.DestOffset     = FP_OFF ( buf );
    semm.DestSegPage   = FP_SEG ( buf );
  } else return( NULL );

  regs.x.si = FP_OFF( &semm );
  regs.x.ax = 0x5700;      /* EMS ﾒﾓﾘ転送 */
  int86 ( 0x67, &regs, &regs );
  if ( regs.h.ah == 0 ) return( -1L );
  pmem->linptr += size;
  return( size );
}

int main (void){
  char buf_s[]  = "test strings";
  char buf_d0[] = "0           ";
  char buf_d1[] = "1           ";
  char __far *buf_e;
  int   i = 0;
  ulong size = 12L;
  EMEM pmem;

  /* ｿｰｽ･ﾊﾞｯﾌｧ の文字列を EMS 経由でﾃﾞｨｽﾄﾈｰｼｮﾝ･ﾊﾞｯﾌｧ に転送 (^^; */

  if ( EMSinit() == 0 ) {
    if ( EMSalloc( size, &pmem ) == 0 ) {
        if (( buf_e = EMSallocmem( &pmem )) != NULL ) {
          while( i<13 ) { buf_e[i] = buf_s[i]; i++; }
        i = 0;
          while( i<13 ) { buf_d0[i] = buf_e[i]; i++; }
        printf ( "%s\n", buf_d0 );
        EMSfree( pmem.handle );
      }
    }
    if ( EMSalloc( size, &pmem ) == 0 ) {
      pmem.linptr = 0L;
      EMSMemMov ( EMSPUT, &pmem, buf_s, size );
      pmem.linptr = 0L;
      EMSMemMov ( EMSGET, &pmem, buf_d1, size );
      printf ( "%s\n", buf_d1 );
      EMSfree( pmem.handle );
    }
  }
  return(0);
}