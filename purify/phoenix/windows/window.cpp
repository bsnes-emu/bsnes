vector<pWindow*> pWindow::modal;

void pWindow::updateModality() {
  for(auto &object : pObject::objects) {
    if(dynamic_cast<pWindow*>(object) == nullptr) continue;
    pWindow *p = (pWindow*)object;
    if(modal.size() == 0) EnableWindow(p->hwnd, true);
    else EnableWindow(p->hwnd, modal.find(p));
  }
}

static const unsigned FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER;
static const unsigned ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

Window& pWindow::none() {
  static Window *window = nullptr;
  if(window == nullptr) window = new Window;
  return *window;
}

void pWindow::append(Layout &layout) {
  Geometry geom = window.state.geometry;
  geom.x = geom.y = 0;
  layout.setGeometry(geom);
}

void pWindow::append(Menu &menu) {
  menu.p.parentWindow = &window;
  updateMenu();
}

void pWindow::append(Widget &widget) {
  widget.p.parentWindow = &window;
  widget.p.orphan();
  if(widget.state.font != "") widget.p.setFont(widget.state.font);
  else if(window.state.widgetFont != "") widget.p.setFont(window.state.widgetFont);
  else widget.p.setFont("Tahoma, 8");
}

Color pWindow::backgroundColor() {
  if(window.state.backgroundColorOverride) return window.state.backgroundColor;
  DWORD color = GetSysColor(COLOR_3DFACE);
  return { (uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)(color >> 0), 255 };
}

bool pWindow::focused() {
  return (GetForegroundWindow() == hwnd);
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

Geometry pWindow::geometry() {
  Geometry margin = frameMargin();

  RECT rc;
  if(IsIconic(hwnd)) {
    //GetWindowRect returns -32000(x),-32000(y) when window is minimized
    WINDOWPLACEMENT wp;
    GetWindowPlacement(hwnd, &wp);
    rc = wp.rcNormalPosition;
  } else {
    GetWindowRect(hwnd, &rc);
  }

  signed x = rc.left + margin.x;
  signed y = rc.top + margin.y;
  unsigned width = (rc.right - rc.left) - margin.width;
  unsigned height = (rc.bottom - rc.top) - margin.height;

  return { x, y, width, height };
}

void pWindow::remove(Layout &layout) {
}

void pWindow::remove(Menu &menu) {
  updateMenu();
}

void pWindow::remove(Widget &widget) {
  widget.p.orphan();
}

void pWindow::setBackgroundColor(const Color &color) {
  if(brush) DeleteObject(brush);
  brushColor = RGB(color.red, color.green, color.blue);
  brush = CreateSolidBrush(brushColor);
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
  for(auto &layout : window.state.layout) {
    Geometry geom = this->geometry();
    geom.x = geom.y = 0;
    layout.setGeometry(geom);
  }
  locked = false;
}

void pWindow::setMenuFont(const string &font) {
}

void pWindow::setMenuVisible(bool visible) {
  locked = true;
  SetMenu(hwnd, visible ? hmenu : 0);
  setGeometry(window.state.geometry);
  locked = false;
}

void pWindow::setModal(bool modality) {
  if(modality == false) {
    if(auto position = modal.find(this)) modal.remove(position());
  } else {
    modal.appendonce(this);
  }
  updateModality();
}

void pWindow::setResizable(bool resizable) {
  SetWindowLongPtr(hwnd, GWL_STYLE, window.state.resizable ? ResizableStyle : FixedStyle);
  setGeometry(window.state.geometry);
}

void pWindow::setStatusFont(const string &font) {
  if(hstatusfont) DeleteObject(hstatusfont);
  hstatusfont = pFont::create(font);
  SendMessage(hstatus, WM_SETFONT, (WPARAM)hstatusfont, 0);
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
  if(visible == false) setModal(false);
}

void pWindow::setWidgetFont(const string &font) {
  for(auto &widget : window.state.widget) {
    if(widget.state.font == "") widget.setFont(font);
  }
}

void pWindow::constructor() {
  brush = 0;

  hwnd = CreateWindow(L"phoenix_window", L"", ResizableStyle, 128, 128, 256, 256, 0, 0, GetModuleHandle(0), 0);
  hmenu = CreateMenu();
  hstatus = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD, 0, 0, 0, 0, hwnd, 0, GetModuleHandle(0), 0);
  hstatusfont = 0;
  setStatusFont("Tahoma, 8");

  //status bar will be capable of receiving tab focus if it is not disabled
  SetWindowLongPtr(hstatus, GWL_STYLE, GetWindowLong(hstatus, GWL_STYLE) | WS_DISABLED);

  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&window);
  setGeometry({ 128, 128, 256, 256 });
}

void pWindow::destructor() {
  DeleteObject(hstatusfont);
  DestroyWindow(hstatus);
  DestroyMenu(hmenu);
  DestroyWindow(hwnd);
}

void pWindow::updateMenu() {
  if(hmenu) DestroyMenu(hmenu);
  hmenu = CreateMenu();

  for(auto &menu : window.state.menu) {
    menu.p.update(window);
    if(menu.visible()) {
      AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)menu.p.hmenu, utf16_t(menu.state.text));
    }
  }

  SetMenu(hwnd, window.state.menuVisible ? hmenu : 0);
}
