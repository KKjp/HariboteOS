# HariboteOS

## make
```
HariboteOS.img    ipl10.S    asmhead.S    bootpack.c    _bootpack.S
      |              |           |            |              |
      |             as          as           gcc            as
      |              |           |            |              |
      |           ipl10.o    asmhead.o    bootpack.o    _bootpack.o
      |              |           |            |              |
      |             ld          ld            -------ld-------
      |              |           |                    |
      |          ipl10.bin   asmhead.bin         bootpack.hrb
      |              |           |                    |
      |<----------mformat        ---------cat----------
      |                                    |
      |                                kernel.bin
      |                                    |
      |<---------------------------------mcopy
      |
     qemu
```

