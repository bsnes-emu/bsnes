long __stdcall window_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(hwnd == wMain.hwnd)     { return wMain.wndproc(msg, wparam, lparam);     }
  if(hwnd == wInputCfg.hwnd) { return wInputCfg.wndproc(msg, wparam, lparam); }
  if(hwnd == wPPUCfg.hwnd)   { return wPPUCfg.wndproc(msg, wparam, lparam);   }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::hide() {
  ShowWindow(hwnd, SW_HIDE);
  settings.visible = false;
}

void Window::show() {
  ShowWindow(hwnd, SW_NORMAL);
  SetFocus(hwnd);
  settings.visible = true;
}

bool Window::visible() {
  return settings.visible;
}

void Window::hide_menu() {
  if(GetMenu(hwnd) == NULL)return;
  SetMenu(hwnd, NULL);

  if(settings.fullscreen == true) {
    if(global::cursor_visible == true) {
      ShowCursor(FALSE);
      global::cursor_visible = false;
    }
    return;
  }

  resize(settings.width, settings.height);
}

void Window::show_menu() {
  if(GetMenu(hwnd) != NULL)return;
  SetMenu(hwnd, menu);

  if(global::cursor_visible == false) {
    ShowCursor(TRUE);
    global::cursor_visible = true;
  }

  if(settings.fullscreen == true)return;
  resize(settings.width, settings.height);
}

//determine if id belongs to a menu, button, etc.
//used by check() routines
uint32 Window::get_id_type(uint32 id) {
HWND ctl = GetDlgItem(hwnd, id);
  if(!ctl)return IDTYPE_MENU;

char t[256];
  GetClassName(GetDlgItem(hwnd, id), t, 255);
  if(strimatch(t, "button"))return IDTYPE_BUTTON;

  return IDTYPE_UNKNOWN;
}

void Window::check(uint32 id, bool state) {
  switch(get_id_type(id)) {
  case IDTYPE_MENU:
    CheckMenuItem(menu, id, (state) ? MF_CHECKED : MF_UNCHECKED);
    break;
  case IDTYPE_BUTTON:
    SendDlgItemMessage(hwnd, id, BM_SETCHECK, (WPARAM)state, 0);
    break;
  }
}

void Window::check_toggle(uint32 id) {
  switch(get_id_type(id)) {
  case IDTYPE_MENU:
    CheckMenuItem(menu, id, (checked(id)) ? MF_UNCHECKED : MF_CHECKED);
    break;
  case IDTYPE_BUTTON:
    SendDlgItemMessage(hwnd, id, BM_SETCHECK,
      (WPARAM)SendDlgItemMessage(hwnd, id, BM_GETCHECK, 0, 0), 0);
    break;
  }
}

bool Window::checked(uint32 id) {
  switch(get_id_type(id)) {
  case IDTYPE_MENU:   return CheckMenuItem(menu, id, 0);
  case IDTYPE_BUTTON: return SendDlgItemMessage(hwnd, id, BM_GETCHECK, 0, 0);
  }
  return false;
}

void Window::check(uint32 id) {
  switch(get_id_type(id)) {
  case IDTYPE_MENU:   CheckMenuItem(menu, id, MF_CHECKED); break;
  case IDTYPE_BUTTON: SendDlgItemMessage(hwnd, id, BM_SETCHECK, (WPARAM)TRUE, 0); break;
  }
}

void Window::uncheck(uint32 id) {
  switch(get_id_type(id)) {
  case IDTYPE_MENU:   CheckMenuItem(menu, id, MF_UNCHECKED); break;
  case IDTYPE_BUTTON: SendDlgItemMessage(hwnd, id, BM_SETCHECK, (WPARAM)FALSE, 0); break;
  }
}

void Window::resize(int width, int height, bool fullscreen) {
int style   = GetWindowLong(hwnd, GWL_STYLE);
bool old_fs = settings.fullscreen;
  settings.width      = width;
  settings.height     = height;
  settings.fullscreen = fullscreen;

  if(fullscreen == false) {
    if(old_fs == true) {
      style |=  WS_CAPTION;
      style |=  WS_SYSMENU;
      style |=  WS_MINIMIZEBOX;
      SetWindowLong(hwnd, GWL_STYLE, style);
      show_menu();
    }

  //add window borders + menu to size
    width  += GetSystemMetrics(SM_CXFIXEDFRAME) << 1;
    height += GetSystemMetrics(SM_CYFIXEDFRAME) << 1;
    height += GetSystemMetrics(SM_CYCAPTION);
    if(GetMenu(hwnd) != 0) {
      height += GetSystemMetrics(SM_CYMENU);
    }
  } else {
    if(old_fs == false) {
      style &= ~WS_CAPTION;
      style &= ~WS_SYSMENU;
      style &= ~WS_MINIMIZEBOX;
      SetWindowLong(hwnd, GWL_STYLE, style);
      hide_menu();
    }
  }

  if(fullscreen == false) {
    SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
  } else {
    SetWindowPos(hwnd, 0, 0, 0,
      GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER);
  }

  if(settings.center == true)center();
}

void Window::center() {
  if(settings.fullscreen == true)return;

RECT rc;
POINT p;
  GetWindowRect(hwnd, &rc);
  p.x = p.y = 0;
  ClientToScreen(hwnd, &p);
  OffsetRect(&rc, p.x, p.y);
int sw = GetSystemMetrics(SM_CXSCREEN),
    sh = GetSystemMetrics(SM_CYSCREEN);
int x = (sw - (rc.right - rc.left)) / 2,
    y = (sh - (rc.bottom - rc.top)) / 2;
  SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::set_text(uint32 id, const char *text, ...) {
  SetDlgItemText(hwnd, id, text);
}

void Window::control(const char *classname, const char *text, uint32 flags, int x, int y, int width, int height, uint32 id) {
  CreateWindow(classname, text, flags | WS_CHILD, x, y, width, height, hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SendDlgItemMessage(hwnd, id, WM_SETFONT, (WPARAM)global::vwf, 0);
}

long Window::wndproc(UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::init(const char *classname, const char *title, int width, int height, HBRUSH hbr, uint32 flags) {
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)hbr;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(100));
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = window_wndproc;
  wc.lpszClassName = classname;
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindowEx((flags & WF_TOPMOST) ? WS_EX_TOPMOST : 0, classname, title,
    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    64, 64, width, height,
    0, 0, wc.hInstance, 0);

  settings.visible = false;
  settings.width   = width;
  settings.height  = height;
  settings.center  = !!(flags & WF_CENTER);
  resize(width, height);
}

Window::Window() {
  hwnd = 0;
  menu = 0;

  settings.fullscreen = false;
  settings.center     = false;
  settings.width      = 320;
  settings.height     = 240;
}
