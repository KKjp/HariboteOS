/* vsprintf()
 * the functions belong to ../include/osutils.h
 */


#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#include "../include/osutils.h"

struct _SPRINTF {
    uint8_t  specifier_chg;
    size_t   width_of_output;
    size_t   value_after_period;
    uint8_t  period : 1;
    struct FLAGS {
        uint8_t plus   : 1;
        uint8_t hash   : 1;
        uint8_t space  : 1;
        uint8_t zero   : 1;
        uint8_t minus  : 1;
    } flags;
};

static int          Make_String_basis_format(struct _SPRINTF *, char *, va_list);
static char        *itostr(char *, const int, const unsigned int);
static char        *itostr_cap16(char *, const int);
static inline char *ptrtostr32(char *, const uint32_t);
static char        *ptrtostr64(char *, const uint64_t);
static int          strtoi3_below_256(const char *);
static char        *makestr_period(char *, const char *, int);


int vsprintf(char *str, const char *fmt, va_list list)
{
    char      add[256] = {0};
    size_t    str_len  = 0;
    size_t    fmt_len  = 0;
    int       ret      = 0;

    struct _SPRINTF output_format = {0};
    
    while (fmt[fmt_len]) {
        if (fmt[fmt_len] == '%') {
            /* initalize to 0 */
            memset(&output_format, 0, sizeof(output_format));
            memset(add, 0, sizeof(add));
            fmt_len++;

            /* read flags */
read_flags:
            switch (fmt[fmt_len]) {
                case ' ' :
                    output_format.flags.space = 1;
                    fmt_len++;
                    goto read_flags;
                case '+' :
                    output_format.flags.plus = 1;
                    fmt_len++;
                    goto read_flags;
                case '-' :
                    output_format.flags.minus = 1;
                    fmt_len++;
                    goto read_flags;
                case '0' :
                    output_format.flags.zero = 1;
                    fmt_len++;
                    goto read_flags;
                case '#' :
                    output_format.flags.hash = 1;
                    fmt_len++;
                    goto read_flags;
                default  :
                    break;
                    /* nopermitable flag */
            }

            /* read the minimum of witdh.
             * 4 digits or more are ignored.
             */

            output_format.width_of_output = strtoi3_below_256(&fmt[fmt_len]);

            while ('0' < fmt[fmt_len] && fmt[fmt_len] < '9') {
                fmt_len++;
            }

            /* read the precision */
            if (fmt[fmt_len] == '.') {
                output_format.period = 1;
                fmt_len++;
                output_format.value_after_period = strtoi3_below_256(&fmt[fmt_len]);
                while ('0' < fmt[fmt_len] && fmt[fmt_len] < '9') {
                    fmt_len++;
                }
            }

            /* read the specifier */
            switch (fmt[fmt_len]) {
                case 'c' :
                case 's' :
                case 'i' :
                case 'd' :
                case 'u' :
                case 'o' :
                case 'x' :
                case 'X' :
                case 'p' :
                    output_format.specifier_chg = fmt[fmt_len];
                    fmt_len++;
                    break;
                case '%' :
                    output_format.specifier_chg = '%';
                    str[str_len++] = '%';
                    fmt_len++;
                    break;
                case 'n' :
                    output_format.specifier_chg = 'n';
                    *(va_arg(list, int *)) = (int) str_len;
                    fmt_len++;
                    break;
                default  :
                    /* not implemented or non-parmitable specifier_chg */
                    fmt_len++;
                    break;
            }

            /* convert the string basis format */
            if (!((ret = Make_String_basis_format(&output_format, add, list)) < 0)) {
                ret = 0;
            }

            /* add string to str[] */
            for (size_t i = 0; add[i]; i++) {
                str[str_len++] = add[i];
            }

        } else {
            /* other charctors */
            str[str_len++] = fmt[fmt_len++];
        }
    }
    
    /* add null charactor */
    str[str_len] = '\0';

    return (ret < 0) ? ret: (int) str_len;
}

#define NON_PERMITED_FLAG(flag) \
    do { if (flag) { flag = 0; ret = -1; } } while (0)

static int Make_String_basis_format(struct _SPRINTF *output_format, char *add, va_list list)
{
    char     buf[256]        = {0};
    char     period_buf[256] = {0};
    size_t   buf_head        = 4;
    int32_t  size            = 0;
    int      ret             = 0;

    switch (output_format->specifier_chg) {
        case 'c' :
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.hash);
            NON_PERMITED_FLAG(output_format->flags.space);
            NON_PERMITED_FLAG(output_format->flags.zero);
            buf[buf_head] = (char) va_arg(list, int);
            break;
        case 's' :
            if (output_format->width_of_output) {
                output_format->width_of_output = 0;
                ret = -1;
            }
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.hash);
            NON_PERMITED_FLAG(output_format->flags.space);
            NON_PERMITED_FLAG(output_format->flags.zero);
            if (!output_format->period) {
                strcat(&buf[buf_head], va_arg(list, char *));
            } else {
                strncat(&buf[buf_head], va_arg(list, char *), output_format->value_after_period);
            }
            break;
        case 'i' :
        case 'd' :
            NON_PERMITED_FLAG(output_format->flags.hash);
            if (output_format->flags.plus) {
                NON_PERMITED_FLAG(output_format->flags.space);
            }
            if (output_format->flags.minus) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            if (output_format->period) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            makestr_period(&buf[buf_head], itostr(&period_buf[0], va_arg(list, int), 10), output_format->value_after_period);
            break;
        case 'u' :
            if (output_format->flags.plus ||
                    output_format->flags.hash                                 ||
                    (output_format->flags.plus && output_format->flags.space) ||
                    (output_format->flags.minus && output_format->flags.zero) ||
                    (output_format->period && output_format->flags.zero)
            ) {
                ret = -1;
            }
            makestr_period(&buf[buf_head], itostr(&period_buf[0], va_arg(list, unsigned int), 10), output_format->value_after_period);
            break;
        case 'o' :
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.space);
            if (output_format->flags.minus) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            if (output_format->period) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            makestr_period(&buf[buf_head], itostr(&period_buf[0], va_arg(list, unsigned int), 8), output_format->value_after_period);
            break;
        case 'x' :
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.space);
            if (output_format->flags.minus) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            if (output_format->period) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            makestr_period(&buf[buf_head], itostr(&period_buf[0], va_arg(list, unsigned int), 16), output_format->value_after_period);
            break;
        case 'X' :
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.space);
            if (output_format->flags.minus) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            if (output_format->period) {
                NON_PERMITED_FLAG(output_format->flags.zero);
            }
            makestr_period(&buf[buf_head], itostr_cap16(&period_buf[0], va_arg(list, unsigned int)), output_format->value_after_period);
            break;
        case 'p' :
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.hash);
            NON_PERMITED_FLAG(output_format->flags.space);
            NON_PERMITED_FLAG(output_format->flags.zero);
            if (output_format->period) {
                output_format->period = 0;
                ret = -1;
            }
#ifdef IS64BIT
            ptrtostr64(&buf[buf_head], (uint64_t) va_arg(list, void *));
#else
            ptrtostr32(&buf[buf_head], (uint32_t) va_arg(list, void *));
#endif
            break;
        case '%' :
        case 'n' :
            /* implemented in vsprintf() */
            NON_PERMITED_FLAG(output_format->flags.plus);
            NON_PERMITED_FLAG(output_format->flags.hash);
            NON_PERMITED_FLAG(output_format->flags.space);
            NON_PERMITED_FLAG(output_format->flags.zero);
            NON_PERMITED_FLAG(output_format->flags.minus);
            return ret;
        case '\0':
            /* the specifier is not exist */
            return -1;
        default  :
            /* the specifier is non-permitable */
            return -1;
    }

    if (buf[0] != '-' &&
            (
                output_format->specifier_chg == 'i' ||
                output_format->specifier_chg == 'd' ||
                output_format->specifier_chg == 'u' ||
                output_format->specifier_chg == 'o' ||
                output_format->specifier_chg == 'x' ||
                output_format->specifier_chg == 'X'
            )
    ) {
        if (output_format->flags.plus) {
            buf[--buf_head] = '+';
            if (output_format->flags.hash) {
                ret = -1;
            }
        } else if (output_format->flags.space) {
            buf[--buf_head] = ' ';
        }

        if (output_format->flags.hash) {
            switch (output_format->specifier_chg) {
                case 'x' :
                    buf[--buf_head] = 'x'; buf[--buf_head] = '0';
                    break;
                case 'X' :
                    buf[--buf_head] = 'X'; buf[--buf_head] = '0';
                    break;
                default  :
                    ret = -1;
                    break;
            }
        }

        if (output_format->flags.minus || output_format->period) {
            output_format->flags.zero = 0;
        }
    }

    if (output_format->specifier_chg == 's' && output_format->value_after_period < strlen(&buf[buf_head])) {
        buf[buf_head + output_format->value_after_period + 1] = '\0';
    }

    if ((size = (int32_t) output_format->width_of_output - strlen(&buf[buf_head])) < 0 || output_format->specifier_chg == 's') {
        size = 0;
    }

    if (!output_format->flags.minus) {
        /* default */
        strncat(&add[size], &buf[buf_head], strlen(&buf[buf_head]));

        if (output_format->flags.zero &&
                output_format->specifier_chg != 's' &&
                output_format->specifier_chg != 'x' &&
                output_format->specifier_chg != 'X'
        ) {
            memset(&add[0], '0', size);
        } else {
            memset(&add[0], ' ', size);
        }
    } else {
        strncat(&add[0], &buf[buf_head], strlen(&buf[buf_head]));

        if (output_format->flags.zero &&
                output_format->specifier_chg != 's' &&
                output_format->specifier_chg != 'x' &&
                output_format->specifier_chg != 'X'
        ) {
            memset(&add[strlen(&buf[buf_head])], '0', size);
        } else {
            memset(&add[strlen(&buf[buf_head])], ' ', size);
        }
    }

    return ret;
}

static char *itostr(char *str, int num, const unsigned int base)
{
    size_t len = 0, i = 0;
    unsigned int buf[64] = {0};

    if (num < 0) {
        str[i++] = '-';
        num = -num;
    }

    for (;;) {
        buf[len++] = num % base;
        if (num < base) break;
        num /= base;
    }
    
    if (base == 10 || base == 8) {
        while (len) {
            str[i++] = buf[--len] + 0x30;
        }
    } else if (base == 16) {
        while (len) {
            str[i++] = (buf[--len] < 10) ? (buf[len] + 0x30) : (buf[len] - 9 + 0x60);
        }
    }

    /* add null charctor */
    str[i] = '\0';

    return str;
}

static char *itostr_cap16(char *str, int num)
{
    size_t len = 0, i = 0;
    unsigned int buf[64] = {0};

    if (num < 0) {
        str[i++] = '-';
        num = -num;
    }

    for (;;) {
        buf[len++] = num % 16;
        if (num < 16) break;
        num /= 16;
    }
    
    while (len) {
        str[i++] = (buf[--len] < 10) ? (buf[len] + 0x30) : (buf[len] - 9 + 0x40);
    }

    /* add null charctor */
    str[i] = '\0';

    return str;
}

static inline char *ptrtostr32(char *str, uint32_t num)
{
    return ptrtostr64(str, (uint64_t) num);
}

static char *ptrtostr64(char *str, uint64_t num)
{
    size_t len = 0, i = 0;
    uint64_t buf[64] = {0};

    /* add prefix */
    *str++ = '0'; *str++ = 'x';

    for (;;) {
        buf[len++] = num % 16;
        if (num < 16) break;
        num /= 16;
    }
    
    while (len) {
        str[i++] = (buf[--len] < 10) ? (buf[len] + 0x30) : (buf[len] - 9 + 0x60);
    }

    /* add null charctor */
    str[i] = '\0';

    return str;
}

static int strtoi3_below_256(const char *str)
{
    char num_buf[3]    = {0};
    int  power_of_10_i = 0;
    int  ret           = 0;
    int  i             = 0;

    for (; '0' < str[i] && str[i] < '9' && i < 3; i++) {
        num_buf[i] = str[i];
    }

    for (int j = i - 1; j >= 0; j--) {
        if (num_buf[j]) {
            /* calculate power_of_10_i */
            if (j == i - 1) {
                power_of_10_i = 1;
            } else if (j == i - 2) {
                power_of_10_i = 10;
            } else if (j == i - 3) {
                power_of_10_i = 100;
            }

            /* convert ascii to integer */
            ret += (num_buf[j] - 0x30) * power_of_10_i;
        }
    }

    return (ret <= 256) ? ret : 256;
}

static char *makestr_period(char *dest, const char *src, int width)
{
    int size = width - strlen(src);

    if (size < 0) { size = 0; }
    strncat(&dest[size], src, strlen(src));
    memset(&dest[0], '0', size);

    return dest;
}

