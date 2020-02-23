#if defined(Hiro_Widget)

namespace hiro {

static auto CALLBACK Widget_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto widget = dynamic_cast<mWidget*>(object)) {
      if(auto self = widget->self()) {
        if(auto result = self->windowProc(hwnd, msg, wparam, lparam)) {
          return result();
        }
        return CallWindowProc(self->defaultWindowProc, hwnd, msg, wparam, lparam);
      }
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pWidget::construct() -> void {
  if(!hwnd) {
    abstract = true;
    hwnd = CreateWindow(L"hiroWidget", L"", WS_CHILD, 0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  }
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  defaultWindowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Widget_windowProc);
  _setState();
}

auto pWidget::destruct() -> void {
  toolTip.reset();
  if(hfont) { DeleteObject(hfont); hfont = nullptr; }
  if(hwnd) { DestroyWindow(hwnd); hwnd = nullptr; }
}

auto pWidget::focused() const -> bool {
  auto focused = GetFocus();
  return hwnd == focused || IsChild(hwnd, focused);
}

auto pWidget::minimumSize() -> Size {
  return {0, 0};
}

auto pWidget::setDroppable(bool droppable) -> void {
  //TODO
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(!self().parentWindow(true)) enabled = false;
  if(!self().enabled(true)) enabled = false;
  if(abstract) enabled = false;
  EnableWindow(hwnd, enabled);
}

auto pWidget::setFocusable(bool focusable) -> void {
  //TODO
}

auto pWidget::setFocused() -> void {
  SetFocus(hwnd);
}

auto pWidget::setFont(const Font&) -> void {
  if(hfont) DeleteObject(hfont);
  hfont = pFont::create(self().font(true));
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  if(auto parent = _parentWidget()) {
    auto displacement = parent->self().geometry().position();
    geometry.setX(geometry.x() - displacement.x());
    geometry.setY(geometry.y() - displacement.y());
  }
  SetWindowPos(hwnd, nullptr, geometry.x(), geometry.y(), geometry.width(), geometry.height(), SWP_NOZORDER);
  //RedrawWindow fixes painting problems when adjusting Layouts manually
  RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN);
  pSizable::setGeometry(geometry);
}

auto pWidget::setMouseCursor(const MouseCursor& mouseCursor) -> void {
  //TODO
}

auto pWidget::setToolTip(const string& toolTipText) -> void {
  if(toolTipText) {
    toolTip = new pToolTip{toolTipText};
  } else {
    toolTip.reset();
  }
}

auto pWidget::setVisible(bool visible) -> void {
  if(!self().parentWindow(true)) visible = false;
  if(!self().visible(true)) visible = false;
  if(abstract) visible = false;
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
}

//

auto pWidget::doMouseHover() -> void {
  if(toolTip) toolTip->show();
}

auto pWidget::doMouseLeave() -> void {
}

auto pWidget::doMouseMove(int x, int y) -> void {
}

auto pWidget::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_SETCURSOR) {
    if(auto cursor = self().mouseCursor()) {
      maybe<LPWSTR> cursorID;
      if(cursor.name() == MouseCursor::Hand) cursorID = IDC_HAND;
      if(cursor.name() == MouseCursor::HorizontalResize) cursorID = IDC_SIZEWE;
      if(cursor.name() == MouseCursor::VerticalResize) cursorID = IDC_SIZENS;
      if(cursorID) return SetCursor(LoadCursor(0, cursorID())), true;
    }
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    //if using a (Horizontal,Vertical)ResizeGrip, it's possible to move the cursor off the widget momentarily.
    //ordinarily, this will cause the cursor to revert to the newly hovered Widget.
    //by capturing the mouse until the mouse button is released, this prevents said cursor flickering.
    //if(msg == WM_LBUTTONDOWN && self().mouseCursor()) SetCapture(hwnd);

    switch(msg) {
    case WM_LBUTTONDOWN: self().doMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: self().doMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: self().doMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    //if(msg == WM_LBUTTONUP && self().mouseCursor() && GetCapture() == hwnd) ReleaseCapture();

    switch(msg) {
    case WM_LBUTTONUP: self().doMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: self().doMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: self().doMouseRelease(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_MOUSEMOVE) {
    TRACKMOUSEEVENT event{sizeof(TRACKMOUSEEVENT)};
    event.hwndTrack = hwnd;
    event.dwFlags = TME_LEAVE | TME_HOVER;
    event.dwHoverTime = pToolTip::Delay;
    TrackMouseEvent(&event);
    POINT p{};
    GetCursorPos(&p);
    doMouseMove(p.x, p.y);
    if(auto toolTip = pApplication::state().toolTip) {
      toolTip->windowProc(hwnd, msg, wparam, lparam);
    }
  }

  if(msg == WM_MOUSELEAVE) {
    doMouseLeave();
  }

  if(msg == WM_MOUSEHOVER) {
    doMouseHover();
  }

  return {};
}

//

auto pWidget::_parentHandle() -> HWND {
  if(auto parent = _parentWidget()) return parent->hwnd;
  if(auto parent = _parentWindow()) return parent->hwnd;
  return nullptr;
}

auto pWidget::_parentWidget() -> maybe<pWidget&> {
  #if defined(Hiro_TabFrame)
  if(auto parent = self().parentTabFrame(true)) {
    if(auto self = parent->self()) return *self;
  }
  #endif
  return {};
}

auto pWidget::_parentWindow() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow(true)) {
    if(auto self = parent->self()) return *self;
  }
  return {};
}

auto pWidget::_setState() -> void {
  setDroppable(self().droppable());
  setEnabled(self().enabled());
  setFocusable(self().focusable());
  setFont(self().font());
  setMouseCursor(self().mouseCursor());
  setToolTip(self().toolTip());
  setVisible(self().visible());
}

}

#endif
