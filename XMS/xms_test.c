#include <stdio.h>
#include <stdlib.h>
// #include <i86.h>
// #include "../00_include/dos_timer.h"
#include "../00_include/xms.h"

int main(void){
  char err;

  puts("Hello XMS!\n");

  /* ====================  /
  /  Setup                 /
  /  ==================== */
  // INSTALLATION CHECK
  err = XMS_exists();
  if(E_HAS_ERROR(err)){
    puts("XMS_exists ERROR: No XMS driver.");
    exit(1);
  }
  puts("XMS driver...OK.");

  // GET DRIVER ADDRESS
  XMS_init();

  // VERSION DUMP (Not required)
  XMS_INFO xms_info;
  XMS_getinfo(&xms_info);
  printf("version=%s rev=%x ", xms_info.var_txt, xms_info.rev);
  printf("hma=%s\n\n", xms_info.hma ? "OK" : "NO");

  // CHECK FREE SPACE (Not required)
  unsigned int max_block, total;
  err = XMS_getFreeSpace(&max_block, &total);
  if(E_HAS_ERROR(err)){
    printf("XMS_getFreeSpace ERROR: code=%02X\n", (err & 0x00FF));
    exit(1);
  }
  printf("=== XMS free space ===\n");
  printf("max_block: %d[KB]  total: %d[KB]\n\n", max_block, total);

  /* ====================  /
  /  Write & Reed test     /
  /  ==================== */
  char *buf_s = "XMS R/W DONE!!";
  char *buf_d = "              ";
  unsigned long size = 14L;
  SMEM mem_p;

  err = XMS_malloc(&mem_p, size);
  if(E_HAS_ERROR(err)){
    printf("XMS_malloc ERROR: code=%02X\n", (err & 0x00FF));
    exit(1);
  }

  XMS_write(&mem_p, buf_s, size);
  mem_p.linptr = 0L;
  XMS_read(&mem_p, buf_d, size);

  printf("%s\n", buf_d);

  err = XMS_free(&mem_p);
  if(E_HAS_ERROR(err)){
    printf("XMS_free ERROR: code=%02X\n", (err & 0x00FF));
    exit(1);
  }

  return 0;
}