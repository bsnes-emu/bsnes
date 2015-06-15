#if defined(Hiro_Widget)

namespace hiro {

auto pWidget::construct() -> void {
  abstract = true;
  //todo: create hiroWidget
  hwnd = CreateWindow(L"hiroLabel", L"", WS_CHILD, 0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  _setState();
}

auto pWidget::destruct() -> void {
  DeleteObject(hfont);
  DestroyWindow(hwnd);
}

auto pWidget::focused() const -> bool {
  auto focused = GetFocus();
  return hwnd == focused || IsChild(hwnd, focused);
}

auto pWidget::minimumSize() -> Size {
  return {0, 0};
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(!self().parentWindow(true)) enabled = false;
  if(!self().enabled(true)) enabled = false;
  if(abstract) enabled = false;
  EnableWindow(hwnd, enabled);
}

auto pWidget::setFocused() -> void {
  SetFocus(hwnd);
}

auto pWidget::setFont(const string&) -> void {
  auto font = self().font(true);
  if(!font) font = Font::sans(8);
  if(hfont) DeleteObject(hfont);
  hfont = pFont::create(font);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  if(auto parent = _parentWidget()) {
    auto displacement = parent->self().geometry().position();
    geometry.setX(geometry.x() - displacement.x());
    geometry.setY(geometry.y() - displacement.y());
  }
  SetWindowPos(hwnd, nullptr, geometry.x(), geometry.y(), geometry.width(), geometry.height(), SWP_NOZORDER);
  self().doSize();
}

auto pWidget::setVisible(bool visible) -> void {
  if(!self().parentWindow(true)) visible = false;
  if(!self().visible(true)) visible = false;
  if(abstract) visible = false;
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
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
  return nothing;
}

auto pWidget::_parentWindow() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow(true)) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pWidget::_setState() -> void {
  setEnabled(self().enabled());
  setFont(self().font());
  setVisible(self().visible());
}

}

#endif
