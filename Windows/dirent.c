#include <windows.h>
#include <stdio.h>
#include <winnls.h>
#include <io.h>
#include "dirent.h"

DIR *opendir(const char *filename)
{
    wchar_t w_filename[MAX_PATH + 3] = {0,};
    unsigned length = MultiByteToWideChar(CP_UTF8, 0, filename, -1, w_filename, MAX_PATH);
    if (length) {
        w_filename[length - 1] = L'/';
        w_filename[length] = L'*';
        w_filename[length + 1] = 0;
    }
    DIR *ret = malloc(sizeof(*ret));
    ret->handle = FindFirstFileW(w_filename, &ret->entry);
    if (ret->handle == INVALID_HANDLE_VALUE) {
        free(ret);
        return NULL;
    }
    
    return ret;
}

int closedir(DIR *dir)
{
    if (dir->handle != INVALID_HANDLE_VALUE) {
        FindClose(dir->handle);
    }
    free(dir);
    return 0;
}

struct dirent *readdir(DIR *dir)
{
    if (dir->handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    
    WideCharToMultiByte(CP_UTF8, 0, dir->entry.cFileName, -1,
                        dir->out_entry.d_name, sizeof(dir->out_entry.d_name),
                        NULL, NULL);
    
    if (!FindNextFileW(dir->handle, &dir->entry)) {
        FindClose(dir->handle);
        dir->handle = INVALID_HANDLE_VALUE;
    }
    
    return &dir->out_entry;
}
