namespace phoenix {

vector<pWindow*> pWindow::modal;

//EnableWindow(hwnd, false) sends WM_KILLFOCUS; deactivating said window
//EnableWindow(hwnd, true) does not restore lost focus
//when a modal loop finishes, and the dialog is dismissed, the application loses focus entirely
//due to anti-focus-stealing code in Windows, SetForegroundWindow() cannot restore lost focus
//further, GetActiveWindow() returns nothing when all windows have lost focus
//thus, we must use a focus-stealing hack to reclaim the focus we never intended to dismiss;
//and we must replicate GetActiveWindow() by scanning the Z-order of windows for this process

void pWindow::updateModality() {
  //bind thread input to process that currently has input focus
  auto threadId = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
  AttachThreadInput(threadId, GetCurrentThreadId(), TRUE);

  pWindow* topMost = nullptr;
  for(auto& object : pObject::objects) {
    if(dynamic_cast<pWindow*>(object) == nullptr) continue;
    pWindow* p = (pWindow*)object;
    bool enable = modal.size() == 0 || modal.find(p);
    if(IsWindowEnabled(p->hwnd) != enable) EnableWindow(p->hwnd, enable);
    if(enable && p->window.visible()) {
      if(topMost == nullptr) topMost = p;
      else if(GetWindowZOrder(p->hwnd) < GetWindowZOrder(topMost->hwnd)) topMost = p;
    }
  }

  //set input focus on top-most window
  if(topMost) {
    SetForegroundWindow(topMost->hwnd);
    SetActiveWindow(topMost->hwnd);
  }

  //unbind thread input hook
  AttachThreadInput(threadId, GetCurrentThreadId(), FALSE);
}

static const unsigned FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER;
static const unsigned ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

Window& pWindow::none() {
  static Window* window = nullptr;
  if(window == nullptr) window = new Window;
  return *window;
}

void pWindow::append(Layout& layout) {
  Geometry geom = window.state.geometry;
  geom.x = geom.y = 0;
  layout.setGeometry(geom);
}

void pWindow::append(Menu& menu) {
  menu.p.parentWindow = &window;
  updateMenu();
}

void pWindow::append(Widget& widget) {
  if(GetParentWidget(&widget)) {
    widget.p.parentHwnd = GetParentWidget(&widget)->p.hwnd;
  } else {
    widget.p.parentHwnd = window.p.hwnd;
  }
  widget.p.orphan();

  if(widget.font().empty() && !window.state.widgetFont.empty()) {
    widget.setFont(window.state.widgetFont);
  }
}

bool pWindow::focused() {
  return (GetForegroundWindow() == hwnd);
}

Geometry pWindow::frameMargin() {
  unsigned style = window.state.resizable ? ResizableStyle : FixedStyle;
  if(window.state.fullScreen) style = 0;
  RECT rc = {0, 0, 640, 480};
  AdjustWindowRect(&rc, style, window.state.menuVisible);
  unsigned statusHeight = 0;
  if(window.state.statusVisible) {
    RECT src;
    GetClientRect(hstatus, &src);
    statusHeight = src.bottom - src.top;
  }
  return {abs(rc.left), abs(rc.top), (rc.right - rc.left) - 640, (rc.bottom - rc.top) + statusHeight - 480};
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

  return {x, y, width, height};
}

void pWindow::remove(Layout& layout) {
}

void pWindow::remove(Menu& menu) {
  updateMenu();
}

void pWindow::remove(Widget& widget) {
  widget.p.orphan();
}

void pWindow::setBackgroundColor(Color color) {
  if(brush) DeleteObject(brush);
  brushColor = RGB(color.red, color.green, color.blue);
  brush = CreateSolidBrush(brushColor);
}

void pWindow::setDroppable(bool droppable) {
  DragAcceptFiles(hwnd, droppable);
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
    setGeometry({margin.x, margin.y, GetSystemMetrics(SM_CXSCREEN) - margin.width, GetSystemMetrics(SM_CYSCREEN) - margin.height});
  }
  locked = false;
}

void pWindow::setGeometry(Geometry geometry) {
  locked = true;
  Geometry margin = frameMargin();
  SetWindowPos(
    hwnd, NULL,
    geometry.x - margin.x, geometry.y - margin.y,
    geometry.width + margin.width, geometry.height + margin.height,
    SWP_NOZORDER | SWP_FRAMECHANGED
  );
  SetWindowPos(hstatus, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);
  for(auto& layout : window.state.layout) {
    Geometry geom = this->geometry();
    geom.x = geom.y = 0;
    layout.setGeometry(geom);
  }
  locked = false;
}

void pWindow::setMenuFont(string font) {
}

void pWindow::setMenuVisible(bool visible) {
  locked = true;
  SetMenu(hwnd, visible ? hmenu : 0);
  setGeometry(window.state.geometry);
  locked = false;
}

void pWindow::setModal(bool modality) {
  if(modality == true) {
    modal.appendonce(this);
    updateModality();
    while(window.state.modal) {
      Application::processEvents();
      usleep(20 * 1000);
    }
    if(auto position = modal.find(this)) modal.remove(position());
    updateModality();
  }
}

void pWindow::setResizable(bool resizable) {
  SetWindowLongPtr(hwnd, GWL_STYLE, window.state.resizable ? ResizableStyle : FixedStyle);
  setGeometry(window.state.geometry);
}

void pWindow::setStatusFont(string font) {
  if(hstatusfont) DeleteObject(hstatusfont);
  hstatusfont = pFont::create(font);
  SendMessage(hstatus, WM_SETFONT, (WPARAM)hstatusfont, 0);
}

void pWindow::setStatusText(string text) {
  SendMessage(hstatus, SB_SETTEXT, 0, (LPARAM)(wchar_t*)utf16_t(text));
}

void pWindow::setStatusVisible(bool visible) {
  locked = true;
  ShowWindow(hstatus, visible ? SW_SHOWNORMAL : SW_HIDE);
  setGeometry(window.state.geometry);
  locked = false;
}

void pWindow::setTitle(string text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pWindow::setVisible(bool visible) {
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
  if(visible == false) setModal(false);
}

void pWindow::setWidgetFont(string font) {
}

void pWindow::constructor() {
  brush = 0;

  hwnd = CreateWindow(L"phoenix_window", L"", ResizableStyle, 128, 128, 256, 256, 0, 0, GetModuleHandle(0), 0);
  hmenu = CreateMenu();
  hstatus = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD, 0, 0, 0, 0, hwnd, 0, GetModuleHandle(0), 0);
  hstatusfont = 0;
  setStatusFont(Font::sans(8));

  //status bar will be capable of receiving tab focus if it is not disabled
  SetWindowLongPtr(hstatus, GWL_STYLE, GetWindowLong(hstatus, GWL_STYLE) | WS_DISABLED);

  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&window);
  setDroppable(window.state.droppable);
  setGeometry({128, 128, 256, 256});

  DWORD color = GetSysColor(COLOR_3DFACE);
  window.state.backgroundColor = Color((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)(color >> 0), 255u);
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

  for(auto& menu : window.state.menu) {
    menu.p.update(window);
    if(menu.visible()) {
      AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)menu.p.hmenu, utf16_t(menu.state.text));
    }
  }

  SetMenu(hwnd, window.state.menuVisible ? hmenu : 0);
}

void pWindow::onClose() {
  if(window.onClose) window.onClose();
  else window.setVisible(false);
  if(window.state.modal && !window.state.visible) window.setModal(false);
}

void pWindow::onDrop(WPARAM wparam) {
  lstring paths = DropPaths(wparam);
  if(paths.empty()) return;
  if(window.onDrop) window.onDrop(paths);
}

bool pWindow::onEraseBackground() {
  if(brush == 0) return false;
  RECT rc;
  GetClientRect(hwnd, &rc);
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  FillRect(ps.hdc, &rc, brush);
  EndPaint(hwnd, &ps);
  return true;
}

void pWindow::onModalBegin() {
  if(Application::Windows::onModalBegin) Application::Windows::onModalBegin();
}

void pWindow::onModalEnd() {
  if(Application::Windows::onModalEnd) Application::Windows::onModalEnd();
}

void pWindow::onMove() {
  if(locked) return;

  Geometry windowGeometry = geometry();
  window.state.geometry.x = windowGeometry.x;
  window.state.geometry.y = windowGeometry.y;

  if(window.onMove) window.onMove();
}

void pWindow::onSize() {
  if(locked) return;
  SetWindowPos(hstatus, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);

  Geometry windowGeometry = geometry();
  window.state.geometry.width = windowGeometry.width;
  window.state.geometry.height = windowGeometry.height;

  for(auto& layout : window.state.layout) {
    Geometry geom = geometry();
    geom.x = geom.y = 0;
    layout.setGeometry(geom);
  }

  if(window.onSize) window.onSize();
}

}
