#pragma once

#ifdef noinline
#undef noinline
#include_next <stdio.h>
#define noinline __attribute__((noinline))
#else
#include_next <stdio.h>
#endif

#include <stdlib.h>
#include <stdarg.h>

int access(const char *filename, int mode);
#define R_OK 4
#define W_OK 2

#ifndef __MINGW32__
#ifndef __LIBRETRO__
static inline int vasprintf(char **str, const char *fmt, va_list args)
{
    size_t size = _vscprintf(fmt, args) + 1;
    *str = (char*)malloc(size);
    int ret = vsprintf(*str, fmt, args);
    if (ret != size - 1) {
        free(*str);
        *str = NULL;
        return -1;
    }
    return ret;
}

static inline int asprintf(char **strp, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int r = vasprintf(strp, fmt, args);
    va_end(args);
    return r;
}

#endif
#endif

/* This code is public domain -- Will Hartung 4/9/09 */
static inline size_t getline(char **lineptr, size_t *n, FILE *stream)
{
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = (char*)malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while (c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = (char*)realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

#define snprintf _snprintf
