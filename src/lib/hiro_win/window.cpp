void pWindow::create(uint style, uint width_, uint height_, const char *text) {
  auto_center = style & Window::AutoCenter;

RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

  hwnd = CreateWindowEx(0, "hiro_window", text ? text : "",
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
    rc.left, rc.top, width_, height_,
    0, 0, GetModuleHandle(0), 0);
  hwndr = CreateWindowEx(0, "hiro_window", text ? text : "",
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
    rc.left, rc.top, width_, height_,
    0, 0, GetModuleHandle(0), 0);
  hmenu = CreateMenu();
  hstatus = CreateWindowEx(0, STATUSCLASSNAME, "",
    WS_CHILD, 0, 0, 0, 0, hwnd, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

  resize(width_, height_);
}

void pWindow::close() {
  CloseWindow(hwnd);
}

void pWindow::move(uint x, uint y) {
  if(is_fullscreen == true) return;
  SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void pWindow::resize(uint width_, uint height_) {
  if(is_fullscreen == true) {
    width_ = GetSystemMetrics(SM_CXSCREEN);
    height_ = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, 0, 0, 0, width_, height_, SWP_NOZORDER | SWP_FRAMECHANGED);
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

int x = (GetSystemMetrics(SM_CXSCREEN) - width_) / 2;
int y = (GetSystemMetrics(SM_CYSCREEN) - height_) / 2;

//if window is larger than screen, force window to top-left corner
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  if(x < rc.left) x = rc.left;
  if(y < rc.top) y = rc.top;

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

uint pWindow::get_width() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  return rc.right - rc.left;
}

uint pWindow::get_height() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  if(status.visible() == false) return rc.bottom - rc.top;
  //do not include statusbar in client area height
  RECT src;
  GetClientRect(hstatus, &src);
  return (rc.bottom - rc.top) - (src.bottom - src.top);
}

void pWindow::set_background_color(uint8_t r, uint8_t g, uint8_t b) {
  if(background) DeleteObject(background);
  background = CreateSolidBrush(RGB(r, g, b));
}

void pWindow::set_text(const char *text) {
  SetWindowText(hwnd, text);
}

void pWindow::attach(Window &window, uint x, uint y) {
  if(!window.p.hwnd) return;

//toplevel window size is larger, because it includes window borders
//read size of window without window borders, and resize upon attach
RECT rc;
  GetClientRect(window.p.hwnd, &rc);

  ShowWindow(window.p.hwnd, SW_HIDE);
  SetWindowLong(window.p.hwnd, GWL_STYLE, WS_CHILD);
  SetParent(window.p.hwnd, hwnd);
  SetWindowPos(window.p.hwnd, 0, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_FRAMECHANGED);
  ShowWindow(window.p.hwnd, SW_NORMAL);
}

void pWindow::attach(MenuGroup &menugroup) {
  AppendMenu(hmenu, MF_STRING | MF_POPUP, (uint)menugroup.p.group, menugroup.p.text);
  if(menu_visible() == false) menu_show();
}

void pWindow::attach(FormControl &formcontrol, uint x, uint y) {
  SetWindowPos(formcontrol.p.hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  SetParent(formcontrol.p.hwnd, hwnd);
}

void pWindow::move(Window &window, uint x, uint y) {
  SetWindowPos(window.p.hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void pWindow::move(FormControl &formcontrol, uint x, uint y) {
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
  hmenu = 0;
  background = 0;
  is_fullscreen = false;
  auto_center = false;
  width = 0;
  height = 0;
}

pWindow::~pWindow() {
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
  SendMessage(hstatus, SB_SETTEXT, 0, (LPARAM)text);
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
