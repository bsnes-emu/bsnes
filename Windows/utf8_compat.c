#include <windows.h>
#include <stdio.h>
#include <winnls.h>

FILE *fopen(const char *filename, const char *mode)
{
    wchar_t w_filename[MAX_PATH] = {0,};
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, w_filename, sizeof(w_filename) / sizeof(w_filename[0]));
    
    wchar_t w_mode[8] = {0,};
    MultiByteToWideChar(CP_UTF8, 0, mode, -1, w_mode, sizeof(w_mode) / sizeof(w_mode[0]));
    
    return _wfopen(w_filename, w_mode);
}