#define _WIN32_IE 0x0600

#include "libui_win.h"
#include "libui_win_window.cpp"
#include "libui_win_control.cpp"

namespace libui {

long __stdcall canvas_wndproc(HWND, UINT, WPARAM, LPARAM);
long __stdcall label_wndproc (HWND, UINT, WPARAM, LPARAM);

HFONT create_font(const char *name, uint size) {
HDC hdc = GetDC(0);
HFONT font = CreateFont(-MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72),
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);
  ReleaseDC(0, hdc);
  return font;
}

void init() {
  InitCommonControls();

WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = libui_wndproc;
  wc.lpszClassName = "libui_class";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  font.variable = create_font("Tahoma", 8);
  font.fixed    = create_font("Courier New", 8);

  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = canvas_wndproc;
  wc.lpszClassName = "canvas_class";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = label_wndproc;
  wc.lpszClassName = "label_class";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
}

void term() {
}

bool run() {
MSG msg;
  if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return events_pending();
}

bool events_pending() {
MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

uint get_screen_width()  { return GetSystemMetrics(SM_CXSCREEN); }
uint get_screen_height() { return GetSystemMetrics(SM_CYSCREEN); }

//

bool file_load(Window *owner, char *filename, const char *filter, const char *path) {
string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

stringarray type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }

char *pf = strptr(f);
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|')pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = owner ? owner->info.hwnd : 0;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.lpstrFile       = filename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetOpenFileName(&ofn);
}

bool file_save(Window *owner, char *filename, const char *filter, const char *path) {
string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

stringarray type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }

char *pf = strptr(f);
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|')pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = owner ? owner->info.hwnd : 0;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.lpstrFile       = filename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetSaveFileName(&ofn);
}

//

uint16 translate_key(uint key) {
  switch(key) {

  case VK_ESCAPE: return keymap::esc;

  case VK_F1:     return keymap::f1;
  case VK_F2:     return keymap::f2;
  case VK_F3:     return keymap::f3;
  case VK_F4:     return keymap::f4;
  case VK_F5:     return keymap::f5;
  case VK_F6:     return keymap::f6;
  case VK_F7:     return keymap::f7;
  case VK_F8:     return keymap::f8;
  case VK_F9:     return keymap::f9;
  case VK_F10:    return keymap::f10;
  case VK_F11:    return keymap::f11;
  case VK_F12:    return keymap::f12;

  case VK_TAB:    return keymap::tab;
  case VK_RETURN: return keymap::enter;
  case VK_SPACE:  return keymap::space;

  case '0':       return keymap::num_0;
  case '1':       return keymap::num_1;
  case '2':       return keymap::num_2;
  case '3':       return keymap::num_3;
  case '4':       return keymap::num_4;
  case '5':       return keymap::num_5;
  case '6':       return keymap::num_6;
  case '7':       return keymap::num_7;
  case '8':       return keymap::num_8;
  case '9':       return keymap::num_9;

  case 'A':       return keymap::a;
  case 'B':       return keymap::b;
  case 'C':       return keymap::c;
  case 'D':       return keymap::d;
  case 'E':       return keymap::e;
  case 'F':       return keymap::f;
  case 'G':       return keymap::g;
  case 'H':       return keymap::h;
  case 'I':       return keymap::i;
  case 'J':       return keymap::j;
  case 'K':       return keymap::k;
  case 'L':       return keymap::l;
  case 'M':       return keymap::m;
  case 'N':       return keymap::n;
  case 'O':       return keymap::o;
  case 'P':       return keymap::p;
  case 'Q':       return keymap::q;
  case 'R':       return keymap::r;
  case 'S':       return keymap::s;
  case 'T':       return keymap::t;
  case 'U':       return keymap::u;
  case 'V':       return keymap::v;
  case 'W':       return keymap::w;
  case 'X':       return keymap::x;
  case 'Y':       return keymap::y;
  case 'Z':       return keymap::z;

  case VK_UP:     return keymap::up;
  case VK_DOWN:   return keymap::down;
  case VK_LEFT:   return keymap::left;
  case VK_RIGHT:  return keymap::right;

  }
  return keymap::none;
}

};
