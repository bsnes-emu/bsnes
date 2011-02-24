static const unsigned FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER;
static const unsigned ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

void pWindow::append(Layout &layout) {
  layout.setParent(window);
  Geometry geom = window.state.geometry;
  geom.x = geom.y = 0;
  layout.setGeometry(geom);
}

void pWindow::append(Menu &menu) {
  updateMenu();
}

void pWindow::append(Widget &widget) {
  widget.p.setParent(window);
}

Geometry pWindow::frameGeometry() {
  RECT rc;
  GetWindowRect(hwnd, &rc);
  return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

bool pWindow::focused() {
  return (GetForegroundWindow() == hwnd);
}

Geometry pWindow::geometry() {
  Geometry margin = frameMargin();
  RECT rc;
  GetWindowRect(hwnd, &rc);

  signed x = rc.left + margin.x;
  signed y = rc.top + margin.y;
  unsigned width = (rc.right - rc.left) - margin.width;
  unsigned height = (rc.bottom - rc.top) - margin.height;

  return { x, y, width, height };
}

void pWindow::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
  if(brush) DeleteObject(brush);
  brushColor = RGB(red, green, blue);
  brush = CreateSolidBrush(brushColor);
}

void pWindow::setFrameGeometry(const Geometry &geometry) {
  Geometry margin = frameMargin();
  window.setGeometry({
    geometry.x + margin.x, geometry.y + margin.y,
    geometry.width - margin.width, geometry.height - margin.height
  });
}

void pWindow::setFocused() {
  if(window.state.visible == false) setVisible(true);
  SetFocus(hwnd);
}

void pWindow::setFullScreen(bool fullScreen) {
  locked = true;
  if(fullScreen == false) {
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | (window.state.resizable ? ResizableStyle : FixedStyle));
    setGeometry(window.state.geometry);
  } else {
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
    Geometry margin = frameMargin();
    setGeometry({ margin.x, margin.y, GetSystemMetrics(SM_CXSCREEN) - margin.width, GetSystemMetrics(SM_CYSCREEN) - margin.height });
  }
  locked = false;
}

void pWindow::setGeometry(const Geometry &geometry) {
  locked = true;
  Geometry margin = frameMargin();
  SetWindowPos(
    hwnd, NULL,
    geometry.x - margin.x, geometry.y - margin.y,
    geometry.width + margin.width, geometry.height + margin.height,
    SWP_NOZORDER | SWP_FRAMECHANGED
  );
  SetWindowPos(hstatus, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);
  foreach(layout, window.state.layout) {
    Geometry geom = this->geometry();
    geom.x = geom.y = 0;
    layout.setGeometry(geom);
  }
  locked = false;
}

void pWindow::setMenuFont(Font &font) {
}

void pWindow::setMenuVisible(bool visible) {
  locked = true;
  SetMenu(hwnd, visible ? hmenu : 0);
  setGeometry(window.state.geometry);
  locked = false;
}

void pWindow::setResizable(bool resizable) {
  SetWindowLongPtr(hwnd, GWL_STYLE, window.state.resizable ? ResizableStyle : FixedStyle);
  setGeometry(window.state.geometry);
}

void pWindow::setStatusFont(Font &font) {
  SendMessage(hstatus, WM_SETFONT, (WPARAM)font.p.hfont, 0);
}

void pWindow::setStatusText(const string &text) {
  SendMessage(hstatus, SB_SETTEXT, 0, (LPARAM)(wchar_t*)utf16_t(text));
}

void pWindow::setStatusVisible(bool visible) {
  locked = true;
  ShowWindow(hstatus, visible ? SW_SHOWNORMAL : SW_HIDE);
  setGeometry(window.state.geometry);
  locked = false;
}

void pWindow::setTitle(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pWindow::setVisible(bool visible) {
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
}

void pWindow::setWidgetFont(Font &font) {
  foreach(widget, window.state.widget) {
    if(!widget.state.font) widget.setFont(font);
  }
}

void pWindow::constructor() {
  brush = 0;

  hwnd = CreateWindow(L"phoenix_window", L"", ResizableStyle, 128, 128, 256, 256, 0, 0, GetModuleHandle(0), 0);
  hmenu = CreateMenu();
  hstatus = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD, 0, 0, 0, 0, hwnd, 0, GetModuleHandle(0), 0);

  //status bar will be capable of receiving tab focus if it is not disabled
  SetWindowLongPtr(hstatus, GWL_STYLE, GetWindowLong(hstatus, GWL_STYLE) | WS_DISABLED);

  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&window);
  setGeometry({ 128, 128, 256, 256 });
}

Geometry pWindow::frameMargin() {
  unsigned style = window.state.resizable ? ResizableStyle : FixedStyle;
  if(window.state.fullScreen) style = 0;
  RECT rc = { 0, 0, 640, 480 };
  AdjustWindowRect(&rc, style, window.state.menuVisible);
  unsigned statusHeight = 0;
  if(window.state.statusVisible) {
    RECT src;
    GetClientRect(hstatus, &src);
    statusHeight = src.bottom - src.top;
  }
  return { abs(rc.left), abs(rc.top), (rc.right - rc.left) - 640, (rc.bottom - rc.top) + statusHeight - 480 };
}

void pWindow::updateMenu() {
  if(hmenu) DestroyMenu(hmenu);
  hmenu = CreateMenu();

  foreach(menu, window.state.menu) {
    menu.p.update(window, hmenu);
    AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)menu.p.hmenu, utf16_t(menu.state.text));
  }

  SetMenu(hwnd, window.state.menuVisible ? hmenu : 0);
}
