#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

typedef BOOL (WINAPI *SLWAProc)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

#ifndef LWA_COLORKEY
#define LWA_COLORKEY 0x00000001
#endif

#ifndef LWA_ALPHA
#define LWA_ALPHA 0x0000002
#endif

BOOL SetWindowAlphaLevel(HWND hwnd, uint level) {
SLWAProc pSLWA;
HMODULE handle = GetModuleHandle("USER32.DLL");
BOOL result;
  if(!handle)return FALSE;
  pSLWA = (SLWAProc)GetProcAddress(handle, (char*)"SetLayeredWindowAttributes");
  if(pSLWA == 0)return FALSE;

  level &= 255;
  if(level != 255) {
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    result = pSLWA(hwnd, 0, level, LWA_ALPHA);
  } else {
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
    result = pSLWA(hwnd, 0, 0, 0);
  }
  InvalidateRect(hwnd, 0, TRUE);
  return result;
}
