#define DBGTOGGLE_BG1ENABLE  100
#define DBGTOGGLE_BG1ENABLE0 101
#define DBGTOGGLE_BG1ENABLE1 102
#define DBGTOGGLE_BG2ENABLE  103
#define DBGTOGGLE_BG2ENABLE0 104
#define DBGTOGGLE_BG2ENABLE1 105
#define DBGTOGGLE_BG3ENABLE  106
#define DBGTOGGLE_BG3ENABLE0 107
#define DBGTOGGLE_BG3ENABLE1 108
#define DBGTOGGLE_BG4ENABLE  109
#define DBGTOGGLE_BG4ENABLE0 110
#define DBGTOGGLE_BG4ENABLE1 111
#define DBGTOGGLE_OAMENABLE  112
#define DBGTOGGLE_OAMENABLE0 113
#define DBGTOGGLE_OAMENABLE1 114
#define DBGTOGGLE_OAMENABLE2 115
#define DBGTOGGLE_OAMENABLE3 116

#define BGTOGGLE_CLICK(src, val)                                       \
  case src:                                                            \
    state = SendDlgItemMessage(hwndDBGToggle, src, BM_GETCHECK, 0, 0); \
    if(state == 0) {                                                   \
      val = true;                                                      \
      SendDlgItemMessage(hwndDBGToggle, src, BM_SETCHECK, 1, 0);       \
    } else {                                                           \
      val = false;                                                     \
      SendDlgItemMessage(hwndDBGToggle, src, BM_SETCHECK, 0, 0);       \
    }                                                                  \
    break

HWND hwndDBGToggle;

long __stdcall wndprocDBGToggle(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
int state;
  if(msg == WM_DESTROY || msg == WM_CLOSE)return 0;
  if(msg == WM_COMMAND) {
    if(HIWORD(wparam) == BN_CLICKED) {
      switch(LOWORD(wparam)) {
        BGTOGGLE_CLICK(DBGTOGGLE_BG1ENABLE,  render.bg1_enabled[DEBUG_BGENABLED_ALL ]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG1ENABLE0, render.bg1_enabled[DEBUG_BGENABLED_PRI0]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG1ENABLE1, render.bg1_enabled[DEBUG_BGENABLED_PRI1]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG2ENABLE,  render.bg2_enabled[DEBUG_BGENABLED_ALL ]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG2ENABLE0, render.bg2_enabled[DEBUG_BGENABLED_PRI0]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG2ENABLE1, render.bg2_enabled[DEBUG_BGENABLED_PRI1]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG3ENABLE,  render.bg3_enabled[DEBUG_BGENABLED_ALL ]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG3ENABLE0, render.bg3_enabled[DEBUG_BGENABLED_PRI0]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG3ENABLE1, render.bg3_enabled[DEBUG_BGENABLED_PRI1]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG4ENABLE,  render.bg4_enabled[DEBUG_BGENABLED_ALL ]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG4ENABLE0, render.bg4_enabled[DEBUG_BGENABLED_PRI0]);
        BGTOGGLE_CLICK(DBGTOGGLE_BG4ENABLE1, render.bg4_enabled[DEBUG_BGENABLED_PRI1]);
        BGTOGGLE_CLICK(DBGTOGGLE_OAMENABLE,  render.oam_enabled[DEBUG_BGENABLED_ALL ]);
        BGTOGGLE_CLICK(DBGTOGGLE_OAMENABLE0, render.oam_enabled[DEBUG_BGENABLED_PRI0]);
        BGTOGGLE_CLICK(DBGTOGGLE_OAMENABLE1, render.oam_enabled[DEBUG_BGENABLED_PRI1]);
        BGTOGGLE_CLICK(DBGTOGGLE_OAMENABLE2, render.oam_enabled[DEBUG_BGENABLED_PRI2]);
        BGTOGGLE_CLICK(DBGTOGGLE_OAMENABLE3, render.oam_enabled[DEBUG_BGENABLED_PRI3]);
      }
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateDBGToggle(void) {
int x, y, wl, wr, h;
  x  =  5;
  y  =  5;
  wl = 90;
  wr = 45;
  h  = 16;
  CreateWindow("BUTTON", "Enable BG1", WS_CHILD|WS_VISIBLE|BS_CHECKBOX,  x, y, wl, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG1ENABLE,  GetModuleHandle(0), 0);
  x += wl;
  CreateWindow("BUTTON", "Pri 0",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG1ENABLE0, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 1",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG1ENABLE1, GetModuleHandle(0), 0);

  x  = 5;
  y += h;
  CreateWindow("BUTTON", "Enable BG2", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wl, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG2ENABLE,  GetModuleHandle(0), 0);
  x += wl;
  CreateWindow("BUTTON", "Pri 0",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG2ENABLE0, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 1",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG2ENABLE1, GetModuleHandle(0), 0);

  x  = 5;
  y += h;
  CreateWindow("BUTTON", "Enable BG3", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wl, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG3ENABLE,  GetModuleHandle(0), 0);
  x += wl;
  CreateWindow("BUTTON", "Pri 0",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG3ENABLE0, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 1",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG3ENABLE1, GetModuleHandle(0), 0);

  x  = 5;
  y += h;
  CreateWindow("BUTTON", "Enable BG4", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wl, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG4ENABLE,  GetModuleHandle(0), 0);
  x += wl;
  CreateWindow("BUTTON", "Pri 0",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG4ENABLE0, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 1",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_BG4ENABLE1, GetModuleHandle(0), 0);

  x  = 5;
  y += h;
  CreateWindow("BUTTON", "Enable OAM", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wl, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_OAMENABLE,  GetModuleHandle(0), 0);
  x += wl;
  CreateWindow("BUTTON", "Pri 0",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_OAMENABLE0, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 1",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_OAMENABLE1, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 2",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_OAMENABLE2, GetModuleHandle(0), 0);
  x += wr;
  CreateWindow("BUTTON", "Pri 3",      WS_CHILD|WS_VISIBLE|BS_CHECKBOX, x, y, wr, h,
    hwndDBGToggle, (HMENU)DBGTOGGLE_OAMENABLE3, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG1ENABLE,  WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG1ENABLE0, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG1ENABLE1, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG2ENABLE,  WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG2ENABLE0, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG2ENABLE1, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG3ENABLE,  WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG3ENABLE0, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG3ENABLE1, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG4ENABLE,  WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG4ENABLE0, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_BG4ENABLE1, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_OAMENABLE,  WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_OAMENABLE0, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_OAMENABLE1, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_OAMENABLE2, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwndDBGToggle, DBGTOGGLE_OAMENABLE3, WM_SETFONT, (WPARAM)hFont, TRUE);

  for(int i=DBGTOGGLE_BG1ENABLE;i<=DBGTOGGLE_OAMENABLE3;i++) {
    SendDlgItemMessage(hwndDBGToggle, i, BM_SETCHECK, 1, 0);
  }
}
