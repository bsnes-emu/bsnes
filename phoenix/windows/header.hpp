#define UNICODE
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600
#define __MSVCRT_VERSION__ 0x0601
#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <io.h>
#include <shlobj.h>
#include <nall/windows/registry.hpp>
#include <nall/windows/utf8.hpp>

#define TBS_TRANSPARENTBKGND 0x1000
