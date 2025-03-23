#include <stdio.h>
#include <unistd.h>

#ifdef USE_MSVCRT_DLL

FILE *__acrt_iob_func(unsigned index)
{
    static FILE *files[3];
    if (index > sizeof(files) / sizeof(files[0])) return NULL;
    if (files[index]) return files[index];
    return (files[index] = fdopen(index, index == STDIN_FILENO? "r" : "w"));
}

#endif