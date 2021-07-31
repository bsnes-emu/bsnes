#include <windows.h>
#include <shlobj.h>
#include "open_dialog.h"

char *do_open_rom_dialog(void)
{
    OPENFILENAMEW dialog;
    static wchar_t filename[MAX_PATH] = {0};
    
    memset(&dialog, 0, sizeof(dialog));
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = filename;
    dialog.nMaxFile = sizeof(filename);
    dialog.lpstrFilter = L"Game Boy ROMs\0*.gb;*.gbc;*.sgb;*.isx\0All files\0*.*\0\0";
    dialog.nFilterIndex = 1;
    dialog.lpstrFileTitle = NULL;
    dialog.nMaxFileTitle = 0;
    dialog.lpstrInitialDir = NULL;
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    if (GetOpenFileNameW(&dialog) == TRUE) { 
        char *ret = malloc(MAX_PATH * 4);
        WideCharToMultiByte(CP_UTF8, 0, filename, sizeof(filename), ret, MAX_PATH * 4, NULL, NULL);
        return ret;
    }
    
    return NULL;
}

char *do_open_folder_dialog(void)
{
    
    BROWSEINFOW dialog;
    memset(&dialog, 0, sizeof(dialog));
    
    dialog.ulFlags = BIF_USENEWUI;
    dialog.lpszTitle = L"Select Boot ROMs Folder";
    
    OleInitialize(NULL);
    
    LPITEMIDLIST list = SHBrowseForFolderW(&dialog);
    static wchar_t filename[MAX_PATH] = {0};

    if (list) {
        if (!SHGetPathFromIDListW(list, filename)) {
            OleUninitialize();
            return NULL;
        }
        char *ret = malloc(MAX_PATH * 4);
        WideCharToMultiByte(CP_UTF8, 0, filename, sizeof(filename), ret, MAX_PATH * 4, NULL, NULL);
        CoTaskMemFree(list);
        OleUninitialize();
        return ret;
    }
    OleUninitialize();
    return NULL;
}
