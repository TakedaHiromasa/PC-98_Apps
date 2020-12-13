# MS-DOS(PC-98) app developed in C language
## Install tools
- [gcc-ia16](https://github.com/tkchia/gcc-ia16)  
```bash
$ sudo add-apt-repository ppa:tkchia/build-ia16
$ sudo apt-get update
$ sudo apt-get install gcc-ia16-elf
$ sudo apt-get install libi86-ia16-elf
```

## How to make
### 
- tiny code model(64KB)  
```
$ ia16-elf-gcc -mcmodel=tiny -march=i286 -o hello.com hello.c
$ file hello.com
hello.com: COM executable for DOS
```

- small code model  
```
$ ia16-elf-gcc -mcmodel=small -march=i286 -o hello.exe hello.c
```

- medium code model  
```
$ ia16-elf-gcc -mcmodel=medium -march=i286 -o hello.exe hello.c
```

- When using libi86  

If any of the headers are used.
```c
#include <bios.h>
#include <conio.h>
#include <dos.h>
#include <dpmi.h>
#include <graph.h>
#include <i86.h>
#include <libi86/string.h>
#include <libi86/stdlib.h>
```

```
$ ia16-elf-gcc -mcmodel=medium -march=i286 -o hello.exe hello.c -li86
```