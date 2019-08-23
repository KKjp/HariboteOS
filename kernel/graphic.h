/* kernel/graphic.h */

#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <stdint.h>

extern void init_palette(void);
extern void init_screen8(char *, const int, const int);
extern void set_palette(const int, const int32_t, const uint8_t *);
extern void putfont8(char *, const int, const int, const int, const char, const char *);
extern void putfonts8_asc(char *, const int, const int, const int, const char, const char *);
extern void boxfill8(char *, const int, const char, const int, const int, const int, const int);
extern void init_mouse_cursor8(char *, const char);
extern void putblock8_8(char *, const int, const int, const int, const int, const int, const char *, const int);

#endif /* GRAPHIC_H_INCLUDED */

