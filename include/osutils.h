/* osutils.h
 * Provide HariboteOS with some utility functions.
 */

#ifndef _OS_UTILS_H_
#define _OS_UTILS_H_

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

extern int     sprintf(char *, const char *, ...);
extern int     vsprintf(char *, const char *, va_list);
extern void    memset(void *, int, size_t);
extern size_t  strlen(const char *);
extern char   *strcat(char *, const char *);
extern char   *strncat(char *, const char *, size_t); 

#endif      /* _OS_UTILS_H_ */

