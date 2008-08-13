void pWindow::create(unsigned style, unsigned width_, unsigned height_, const char *text) {
  auto_center = style & Window::AutoCenter;

  RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

  hwnd = CreateWindowEx(0, L"hiro_window", utf16(text),
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
    rc.left, rc.top, width_, height_,
    0, 0, GetModuleHandle(0), 0);
  hwndr = CreateWindowEx(0, L"hiro_window", utf16(text),
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
    rc.left, rc.top, width_, height_,
    0, 0, GetModuleHandle(0), 0);
  hmenu = CreateMenu();
  hstatus = CreateWindowEx(0, STATUSCLASSNAME, L"",
    WS_CHILD, 0, 0, 0, 0, hwnd, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

  resize(width_, height_);
}

void pWindow::close() {
  CloseWindow(hwnd);
}

void pWindow::move(unsigned x, unsigned y) {
  if(is_fullscreen == true) return;
  SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void pWindow::resize(unsigned width_, unsigned height_) {
  int screen_width = GetSystemMetrics(SM_CXSCREEN);
  int screen_height = GetSystemMetrics(SM_CYSCREEN);

  if(is_fullscreen == true) {
    SetWindowPos(hwnd, 0, 0, 0, screen_width, screen_height, SWP_NOZORDER | SWP_FRAMECHANGED);
    return;
  }

  width = width_;
  height = height_;

  //set requested window size to hidden window, calculate the difference between
  //requested and actual client size area, and then adjust width so that new
  //width, height values will set requested client area size.
  //AdjustWindowRect() does not properly calculate the height of multi-line menus,
  //and thusly is not used.
  SetWindowPos(hwndr, 0, 0, 0, width_, height_, SWP_NOMOVE | SWP_NOZORDER);
  RECT rc;
  GetClientRect(hwndr, &rc);
  width_ += width_ - (rc.right - rc.left);
  height_ += height_ - (rc.bottom - rc.top);

  if(status.visible()) {
    //statusbar does not count as part of window client area width, height
    GetClientRect(hstatus, &rc);
    height_ += rc.bottom - rc.top;
  }

  //if window is larger than the screen size, Windows will hide the window entirely.
  //therefore, window must be constrained to fit within the current screen size.

  int x, y;
  if(width_ <= screen_width) {
    x = (screen_width - width_) >> 1;
  } else {
    x = 0;
    width_ = screen_width;
  }

  if(height_ <= screen_height) {
    y = (screen_height - height_) >> 1;
  } else {
    y = 0;
    height_ = screen_height;
  }

  SetWindowPos(hwnd, 0, x, y, width_, height_, (auto_center ? 0 : SWP_NOMOVE) | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void pWindow::focus() {
  if(visible() == false) show();
  SetFocus(hwnd);
}

bool pWindow::focused() {
  return GetForegroundWindow() == hwnd;
}

void pWindow::show(bool state) {
  if(state == true) {
    ShowWindow(hwnd, SW_NORMAL);
    SetFocus(hwnd);
  } else {
    ShowWindow(hwnd, SW_HIDE);
  }
}

void pWindow::hide() {
  show(false);
}

bool pWindow::visible() {
  return GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE;
}

void pWindow::fullscreen() {
  if(is_fullscreen == true) return;
  is_fullscreen = true;
  SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
  resize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void pWindow::unfullscreen() {
  if(is_fullscreen == false) return;
  is_fullscreen = false;
  SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE);
  resize(width, height);
}

unsigned pWindow::get_width() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  return rc.right - rc.left;
}

unsigned pWindow::get_height() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  if(status.visible() == false) return rc.bottom - rc.top;
  //do not include statusbar in client area height
  RECT src;
  GetClientRect(hstatus, &src);
  return (rc.bottom - rc.top) - (src.bottom - src.top);
}

void pWindow::set_opacity(uint8_t opacity_) {
  opacity = opacity_;
  if(!hwnd) return;

  if(opacity != 255) {
    //enable translucency
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, opacity, LWA_ALPHA);
  } else {
    //disable transluceny
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, 0, 0);
  }

  InvalidateRect(hwnd, 0, TRUE);
}

void pWindow::set_background_color(uint8_t r, uint8_t g, uint8_t b) {
  if(background) DeleteObject(background);
  background = CreateSolidBrush(RGB(r, g, b));
}

void pWindow::set_icon(unsigned width, unsigned height, const uint32_t *data) {
  if(hicon) DestroyIcon(hicon);

  uint8_t *mask = (uint8_t*)malloc(width * height / 8);
  memset(mask, 0, width * height / 8);
  uint8_t *icon = (uint8_t*)malloc(width * height * 4);
  memcpy(icon, data, width * height * 4);

  if((phiro().osversioninfo.dwMajorVersion < 5)
  || (phiro().osversioninfo.dwMajorVersion == 5 && phiro().osversioninfo.dwMinorVersion < 1)) {
    //5.1 and above represents Windows XP or later, which supports 32-bit icons.
    //5.0 and below represents Windows 2000 or earlier, which does not support 32-bit icons.
    //if running Win2k or prior, alpha channel will be ignored;
    //so scale color intensity by alpha level, which gives icons a black background.
    //this is because an alpha of 0 (fully transparent) results in a color of 0 (black).
    //without this step and alpha ignored, icons lose appearance of anti-aliasing.
    for(unsigned i = 0; i < width * height; i++) {
      uint8_t a = icon[i * 4 + 3];
      uint8_t r = uint8_t(1.0 / 256.0 * a * icon[i * 4 + 2]);
      uint8_t g = uint8_t(1.0 / 256.0 * a * icon[i * 4 + 1]);
      uint8_t b = uint8_t(1.0 / 256.0 * a * icon[i * 4 + 0]);
      icon[i * 4 + 3] = 0xff; //ignored anyway, but just to be safe ...
      icon[i * 4 + 2] = max(0, min(255, r)); //clamp 0 <= color <= 255 ...
      icon[i * 4 + 1] = max(0, min(255, g)); //not required, but again ...
      icon[i * 4 + 0] = max(0, min(255, b)); //it's better to be safe.
    }
  }

  hicon = CreateIcon(GetModuleHandle(0), width, height, 1, 32, (const BYTE*)mask, (const BYTE*)icon);
  SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
  SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);

  free(mask);
  free(icon);
}

void pWindow::set_text(const char *text) {
  SetWindowText(hwnd, utf16(text));
}

void pWindow::attach(Window &window, unsigned x, unsigned y) {
  if(!window.p.hwnd) return;

  //toplevel window size is larger, because it includes window borders
  //read size of window without window borders, and resize upon attach
  RECT rc;
  GetClientRect(window.p.hwnd, &rc);

  ShowWindow(window.p.hwnd, SW_HIDE);
  SetWindowLong(window.p.hwnd, GWL_STYLE, WS_CHILD);
  SetWindowLong(window.p.hwnd, GWL_EXSTYLE, WS_EX_CONTROLPARENT);
  SetParent(window.p.hwnd, hwnd);
  SetWindowPos(window.p.hwnd, 0, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_FRAMECHANGED);
  ShowWindow(window.p.hwnd, SW_NORMAL);
}

void pWindow::attach(MenuGroup &menugroup) {
  AppendMenu(hmenu, MF_STRING | MF_POPUP, (unsigned)menugroup.p.group, utf16(menugroup.p.text));
  if(menu_visible() == false) menu_show();
}

void pWindow::attach(FormControl &formcontrol, unsigned x, unsigned y) {
  SetWindowPos(formcontrol.p.hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  SetParent(formcontrol.p.hwnd, hwnd);
  //SetParent() sets Z-order to topmost ...
  //this causes WS_TABSTOP property to run through controls "backward"
  //by inverting this, the later a control is attached, the later the tab key moves to it
  SetWindowPos(formcontrol.p.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void pWindow::move(Window &window, unsigned x, unsigned y) {
  SetWindowPos(window.p.hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void pWindow::move(FormControl &formcontrol, unsigned x, unsigned y) {
  SetWindowPos(formcontrol.p.hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

/* pWindow -> Menubar */

void pWindow::Menubar::show(bool state) {
  p.menu_show(state);
}

void pWindow::Menubar::hide() {
  p.menu_hide();
}

bool pWindow::Menubar::visible() {
  return p.menu_visible();
}

pWindow::Menubar::Menubar(pWindow &p_) : p(p_) {
}

/* pWindow -> Statusbar */

void pWindow::Statusbar::set_text(const char *text) {
  p.status_set_text(text);
}

void pWindow::Statusbar::show(bool state) {
  p.status_show(state);
}

void pWindow::Statusbar::hide() {
  p.status_hide();
}

bool pWindow::Statusbar::visible() {
  return p.status_visible();
}

pWindow::Statusbar::Statusbar(pWindow &p_) : p(p_) {
}

pWindow::pWindow(Window &self_) : pWidget(self_), self(self_), menu(*this), status(*this) {
  hwnd = 0;
  hwndr = 0;
  hicon = 0;
  hmenu = 0;
  background = 0;
  opacity = 255;
  is_fullscreen = false;
  auto_center = false;
  width = 0;
  height = 0;
}

pWindow::~pWindow() {
  if(hicon) DestroyIcon(hicon);
  if(background) DeleteObject(background);
}

/* internal */

uintptr_t pWindow::handle() {
  return (uintptr_t)hwnd;
}

void pWindow::menu_show(bool state) {
  if(state) {
    SetMenu(hwnd, hmenu);
    SetMenu(hwndr, hmenu);
  } else {
    SetMenu(hwnd, 0);
    SetMenu(hwndr, 0);
  }
  resize(width, height);
}

void pWindow::menu_hide() {
  menu_show(false);
}

bool pWindow::menu_visible() {
  return GetMenu(hwnd);
}

void pWindow::status_set_text(const char *text) {
  SendMessage(hstatus, SB_SETTEXT, 0, (LPARAM)(wchar_t*)utf16(text));
}

void pWindow::status_show(bool state) {
  ShowWindow(hstatus, state ? SW_SHOWNORMAL : SW_HIDE);
  resize(width, height);
}

void pWindow::status_hide() {
  status_show(false);
}

bool pWindow::status_visible() {
  return GetWindowLong(hstatus, GWL_STYLE) & WS_VISIBLE;
}
