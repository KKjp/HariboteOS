# HariboteOS
   
## environments
```
$ uname -a
Linux *** 4.15.0-47-generic #50-Ubuntu SMP Wed Mar 13 10:44:52 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux
$ as --version
GNU assembler (GNU Binutils for Ubuntu) 2.30
***
$ gcc --version
gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
***
$ qemu-system-i386 --version
QEMU emulator version 2.11.1(Debian 1:2.11+dfsg-1ubuntu7.12)
***
$ ld --version
GNU ld (GNU Binutils for Ubuntu) 2.30
***
$ make --version
GNU Make 4.1
***
```

## make
```
HariboteOS.img    ipl10.S
      |              |
      |             as
      |              |
      |           ipl10.o
      |              |
      |             ld
      |              |
      |          ipl10.bin
      |              |
      |<----------mformat
      |
      |          asmhead.S     bootpack.c    _bootpack.S
      |              |             |              |
      |             as            gcc            as
      |              |             |              |
      |          asmhead.o     bootpack.o    _bootpack.o
      |              |             |              |
      |             ld             |  |-----------|
      |              |             |  |
      |         asmhead.bin        |  |
      |              |             |  |
      |              |  |----------|  |
      |              |  |  |----------|
      |              |  |  |
      |              |  |  |   hankaku.txt    makefont.c
      |              |  |  |        |             |
      |              |  |  |        |            gcc
      |              |  |  |        |             |
      |              |  |  |     makefont<--------|
      |              |  |  |        |
      |              |  |  |    hankaku.S
      |              |  |  |        |
      |              |  |  |       as
      |              |  |  |        |
      |              |  |  |    hankaku.o
      |              |  |  |        |
      |              |  |  |        |-------------|
      |              |  |  |                      |
      |              |  |  |--------|             |
      |              |  |           |             |
      |              |  |----------ld-------------|
      |              |              |
      |              |         bootpack.hrb
      |              |              |
      |              |-----cat------|
      |                     |
      |                kernel.bin
      |                     |
      |<------------------mcopy
      |
    qemu
```

