#include "../base.h"
#include "../timing/timing.h"
extern snes_timer *snes_time;
#include "../cpu/g65816.h"
extern g65816 *gx816;
extern emustate emu_state;
extern debugstate debugger;
extern videostate render;
extern ppustate ppu;
#include <windows.h>

#define BSNES_TITLE "bsnes v0.0.002 ir9"

enum {
  MENU_FILE_LOAD = 100,
  MENU_FILE_RESET,
  MENU_FILE_EXIT,
  MENU_SETTINGS_VIDEOMODE_512x448w,
  MENU_SETTINGS_VIDEOMODE_640x480f,
  MENU_SETTINGS_FRAMESKIP_OFF,
  MENU_SETTINGS_FRAMESKIP_1,
  MENU_SETTINGS_FRAMESKIP_2,
  MENU_SETTINGS_FRAMESKIP_3,
  MENU_SETTINGS_FRAMESKIP_4,
  MENU_SETTINGS_FRAMESKIP_5,
  MENU_SETTINGS_FRAMESKIP_6,
  MENU_SETTINGS_FRAMESKIP_7,
  MENU_SETTINGS_FRAMESKIP_8,
  MENU_SETTINGS_FRAMESKIP_9,
  MENU_SETTINGS_DEBUGGER,
  MENU_HELP_ABOUT
};

HWND  hwndMain = 0;
HMENU hmenuMain;
HFONT hFontFixed, hFont;

extern joypad_state joypad1;

#define KEY_UP    VK_UP
#define KEY_DOWN  VK_DOWN
#define KEY_LEFT  VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_SHIFT VK_RSHIFT
#define KEY_ENTER VK_RETURN
#define KEY_A     'A'
#define KEY_S     'S'
#define KEY_D     'D'
#define KEY_Z     'Z'
#define KEY_X     'X'
#define KEY_C     'C'

#define KeyState(key) ((GetAsyncKeyState(key) & 0x8000)?1:0)

void UpdateJoypad(void) {
  joypad1.up     = KeyState(KEY_UP   );
  joypad1.down   = KeyState(KEY_DOWN );
  joypad1.left   = KeyState(KEY_LEFT );
  joypad1.right  = KeyState(KEY_RIGHT);
  joypad1.select = KeyState(KEY_SHIFT);
  joypad1.start  = KeyState(KEY_ENTER);
  joypad1.y      = KeyState(KEY_A    );
  joypad1.b      = KeyState(KEY_Z    );
  joypad1.x      = KeyState(KEY_S    );
  joypad1.a      = KeyState(KEY_X    );
  joypad1.l      = KeyState(KEY_D    );
  joypad1.r      = KeyState(KEY_C    );
}

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  MessageBox(0, str, "bsnes", MB_OK);
}

void FixWindowSize(HWND hwnd, ulong width, ulong height, ulong px = null, ulong py = null) {
RECT rc;
ulong x, y, wx, wy;
  ShowWindow(hwnd, SW_HIDE);
  SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER);
  GetClientRect(hwnd, &rc);
  x = width + width - (rc.right - rc.left);
  y = height + height - (rc.bottom - rc.top);
  wx = (GetSystemMetrics(SM_CXSCREEN) - x) / 2;
  wy = (GetSystemMetrics(SM_CYSCREEN) - y) / 2;
  if(px == null || py == null) {
    SetWindowPos(hwnd, 0, wx, wy, x, y, SWP_NOZORDER);
  } else {
    SetWindowPos(hwnd, 0, px, py, x, y, SWP_NOZORDER);
  }
}

HBRUSH black_brush;

long __stdcall wndprocMain(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void RegisterMainWindow() {
WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = black_brush;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndprocMain;
  wc.lpszClassName = "bsnes";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
}

void CreateMainMenu(void) {
HMENU hsubmenu, hbranchmenu;
  hmenuMain = CreateMenu();

  hsubmenu = CreatePopupMenu();
  AppendMenu(hsubmenu, MF_STRING, MENU_FILE_LOAD,  "&Load ROM");
  AppendMenu(hsubmenu, MF_STRING, MENU_FILE_RESET, "&Reset");
  AppendMenu(hsubmenu, MF_SEPARATOR, 0, "");
  AppendMenu(hsubmenu, MF_STRING, MENU_FILE_EXIT, "E&xit");
  AppendMenu(hmenuMain, MF_STRING | MF_POPUP, (unsigned int)hsubmenu, "&File");

  hsubmenu = CreatePopupMenu();

  hbranchmenu = CreatePopupMenu();
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_VIDEOMODE_512x448w, "512x448 Windowed");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_VIDEOMODE_640x480f, "640x480 Fullscreen");
  AppendMenu(hsubmenu, MF_STRING | MF_POPUP, (unsigned int)hbranchmenu, "&Video Mode");

  hbranchmenu = CreatePopupMenu();
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_OFF, "Off");
  AppendMenu(hbranchmenu, MF_SEPARATOR, 0, "");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_1,     "1");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_2,     "2");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_3,     "3");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_4,     "4");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_5,     "5");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_6,     "6");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_7,     "7");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_8,     "8");
  AppendMenu(hbranchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_9,     "9");
  AppendMenu(hsubmenu, MF_STRING | MF_POPUP, (unsigned int)hbranchmenu, "&Frameskip");

  AppendMenu(hsubmenu, MF_SEPARATOR, 0, "");
  AppendMenu(hsubmenu, MF_STRING | (debug_get_state() == DEBUGMODE_WAIT)?MF_CHECKED:MF_UNCHECKED, MENU_SETTINGS_DEBUGGER, "&Debug Mode");
  AppendMenu(hmenuMain, MF_STRING | MF_POPUP, (unsigned int)hsubmenu, "&Settings");

  hsubmenu = CreatePopupMenu();
  AppendMenu(hsubmenu, MF_STRING, MENU_HELP_ABOUT, "&About");
  AppendMenu(hmenuMain, MF_STRING | MF_POPUP, (unsigned int)hsubmenu, "&Help");

  CheckMenuItem(hmenuMain, MENU_SETTINGS_VIDEOMODE_512x448w, MF_CHECKED);
  CheckMenuItem(hmenuMain, MENU_SETTINGS_FRAMESKIP_OFF + render.frame_skip, MF_CHECKED);
}

void CreateMainWindow(void) {
  hwndMain = CreateWindow("bsnes", BSNES_TITLE, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, 640, 480, 0, 0, GetModuleHandle(0), 0);
  CreateMainMenu();
}

void SetMainWindowPos(bool update_style) {
  if(render.fullscreen == true) {
    if(update_style == true) {
      SetWindowLong(hwndMain, GWL_STYLE,   WS_POPUP);
      SetWindowLong(hwndMain, GWL_EXSTYLE, WS_EX_TOPMOST);
      SetWindowPos(hwndMain, HWND_TOPMOST, 0, 0, 640, 480, 0);
    }
    if(render.show_menu == true) {
      ShowCursor(TRUE);
    } else {
      ShowCursor(FALSE);
    }
  } else {
    if(update_style == true) {
      SetWindowLong(hwndMain, GWL_STYLE,   WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
      SetWindowLong(hwndMain, GWL_EXSTYLE, 0);
    }
    FixWindowSize(hwndMain, render.width, render.height);
  }
}

void UpdateMainWindowStyle(bool update_style) {
  if(render.fullscreen == false) {
    ShowWindow(hwndMain, SW_HIDE);
  }

  if(render.show_menu == true) {
    SetMenu(hwndMain, hmenuMain);
    SetMainWindowPos(update_style);
  } else {
    SetMenu(hwndMain, 0);
    SetMainWindowPos(update_style);
  }

  if(render.fullscreen == false) {
    ShowWindow(hwndMain, SW_NORMAL);
  }
}

HWND NewWindow(WNDPROC wndproc, char *classname, char *title, ulong color, ulong width, ulong height) {
WNDCLASS wc;
HWND hwnd;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (color == null)?(HBRUSH)(COLOR_WINDOW):black_brush;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndproc;
  wc.lpszClassName = classname;
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindow(classname, title, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, width, height, 0, 0, wc.hInstance, 0);
  return hwnd;
}

#include "render.cpp"

bool GUIOpenFile(char *fn) {
OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwndMain;
  ofn.lpstrFilter = "SNES ROM Images (*.smc;*.swc;*.fig;*.ufo;*.gd3;*.078)\0*.smc;*.swc;*.fig;*.ufo;*.gd3;*.078\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = fn;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt = "smc";

  if(GetOpenFileName(&ofn)) {
    return true;
  } else {
    return false;
  }
}

void EnableDebugger(byte first_time);
void DisableDebugger(void);

long __stdcall wndprocMain(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char fn[MAX_PATH];
bool result;
  switch(msg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_PAINT:
    UpdateDisplay_NoRender();
    break;
  case WM_KEYDOWN:
    switch(wparam) {
    case VK_ESCAPE:
      if(render.show_menu == true) {
        render.show_menu = false;
      } else {
        render.show_menu = true;
      }
      UpdateMainWindowStyle(false);
      break;
    }
    break;
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case MENU_FILE_LOAD:
      strcpy(fn, "");
      result = GUIOpenFile(fn);
      if(result == true) {
        strcpy(emu_state.rom_name, fn);
        fn[strlen(fn) - 4] = 0;
        strcat(fn, ".srm");
        strcpy(emu_state.sram_name, fn);
        gx816->PowerOn(0);
        gx816->LoadROM();
        if(debug_get_state() == DEBUGMODE_NOROM) {
          debug_set_state(DEBUGMODE_DISABLED);
        }
      }
      break;
    case MENU_FILE_RESET:
      gx816->Reset();
      break;
    case MENU_FILE_EXIT:
      PostQuitMessage(0);
      break;
    case MENU_SETTINGS_VIDEOMODE_512x448w:
      video_setmode(false, 512, 448);
      CheckMenuItem(hmenuMain, MENU_SETTINGS_VIDEOMODE_512x448w, MF_CHECKED);
      CheckMenuItem(hmenuMain, MENU_SETTINGS_VIDEOMODE_640x480f, MF_UNCHECKED);
      break;
    case MENU_SETTINGS_VIDEOMODE_640x480f:
      video_setmode(true, 512, 448);
      CheckMenuItem(hmenuMain, MENU_SETTINGS_VIDEOMODE_512x448w, MF_UNCHECKED);
      CheckMenuItem(hmenuMain, MENU_SETTINGS_VIDEOMODE_640x480f, MF_CHECKED);
      break;
    case MENU_SETTINGS_FRAMESKIP_OFF:
    case MENU_SETTINGS_FRAMESKIP_1:
    case MENU_SETTINGS_FRAMESKIP_2:
    case MENU_SETTINGS_FRAMESKIP_3:
    case MENU_SETTINGS_FRAMESKIP_4:
    case MENU_SETTINGS_FRAMESKIP_5:
    case MENU_SETTINGS_FRAMESKIP_6:
    case MENU_SETTINGS_FRAMESKIP_7:
    case MENU_SETTINGS_FRAMESKIP_8:
    case MENU_SETTINGS_FRAMESKIP_9:
      CheckMenuItem(hmenuMain, MENU_SETTINGS_FRAMESKIP_OFF + render.frame_skip, MF_UNCHECKED);
      render.frame_skip  = LOWORD(wparam) - MENU_SETTINGS_FRAMESKIP_OFF;
      render.frame_count = 0;
      CheckMenuItem(hmenuMain, MENU_SETTINGS_FRAMESKIP_OFF + render.frame_skip, MF_CHECKED);
      break;
    case MENU_SETTINGS_DEBUGGER:
      if(debug_get_state() == DEBUGMODE_NOROM)break;

      if(debug_get_state() == DEBUGMODE_DISABLED) {
        CheckMenuItem(hmenuMain, MENU_SETTINGS_DEBUGGER, MF_CHECKED);
        EnableDebugger(0);
      } else {
        CheckMenuItem(hmenuMain, MENU_SETTINGS_DEBUGGER, MF_UNCHECKED);
        DisableDebugger();
      }
      break;
    case MENU_HELP_ABOUT:
      MessageBox(hwndMain, "bsnes -- written by byuu", "About", MB_OK);
      break;
    }
    break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateFonts(void) {
HDC hdc;
long height;
  hdc = GetDC(0);
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hFontFixed = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");

  hdc = GetDC(0);
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hFont      = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
}

#include "gui_cpu.cpp"
#include "gui_mem.cpp"
#include "gui_bp.cpp"
#include "gui_bgtoggle.cpp"

void EnableDebugger(byte first_time) {
  debug_set_state(DEBUGMODE_WAIT);

  hwndDCPU = NewWindow(wndprocDCPU, "bsnes_cpu", "g65816 cpu debugger", null, DCPU_WIDTH, DCPU_HEIGHT);
  CreateDCPU();
  FixWindowSize(hwndDCPU, DCPU_WIDTH, DCPU_HEIGHT, 0, 1024 - 320);

  hwndDMEM = NewWindow(wndprocDMEM, "bsnes_mem", "g65816 memory editor -- snes memory mode", null, 495, 238);
  CreateDMEM();
  FixWindowSize(hwndDMEM, 495, 238, 316, 441);

  hwndDBP = NewWindow(wndprocDBP, "bsnes_bp", "g65816 breakpoint editor", null, 310, 238);
  CreateDBP();
  FixWindowSize(hwndDBP, 310, 238, 0, 441);

  hwndDBGToggle = NewWindow(wndprocDBGToggle, "bsnes_bgtoggle", "ppu bg toggle", null, 275, 90);
  CreateDBGToggle();
  FixWindowSize(hwndDBGToggle, 275, 90, 0, 326);

  FixWindowSize(hwndMain, 256, 224, 681, 1024 - 320);

  ShowWindow(hwndDCPU,      SW_NORMAL);
  ShowWindow(hwndDMEM,      SW_NORMAL);
  ShowWindow(hwndDBP,       SW_NORMAL);
  ShowWindow(hwndDBGToggle, SW_NORMAL);
  ShowWindow(hwndMain,      SW_NORMAL);

  if(first_time == 0) {
    debug_refresh_mem();
    debug_refresh_bp();
    debug_update_cycles();
    dprintf("* Debugger Enabled");
    UpdateDisplay();
  }
}

void DisableDebugger(void) {
  debug_set_state(DEBUGMODE_DISABLED);

  DestroyWindow(hwndDCPU);
  DestroyWindow(hwndDBP);
  DestroyWindow(hwndDMEM);
  DestroyWindow(hwndDBGToggle);
}

void __winmain(void) {
MSG msg;
  CreateFonts();
  black_brush = CreateSolidBrush(RGB(0, 0, 0));
  RegisterMainWindow();
  CreateMainWindow();
  UpdateMainWindowStyle(false);

  if(debug_get_state() == DEBUGMODE_WAIT) {
    EnableDebugger(1);
  }
  video_setmode(render.fullscreen, render.width, render.height);

  InitDisplay();
  CreateColorTable();
  SelectRenderer();

  InitSNES();

  UpdateDisplay();
  UpdateDisplay();

  while(1) {
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_QUIT)break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      RunSNES();
    }
  }
}
