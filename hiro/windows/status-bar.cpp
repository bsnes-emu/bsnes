#if defined(Hiro_StatusBar)

namespace hiro {

auto pStatusBar::construct() -> void {
  if(auto parent = _parent()) {
    hwnd = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD | WS_DISABLED, 0, 0, 0, 0, parent->hwnd, nullptr, GetModuleHandle(0), 0);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
    setEnabled(self().enabled(true));
    setFont(self().font(true));
    setText(self().text());
    setVisible(self().visible(true));
  }
}

auto pStatusBar::destruct() -> void {
  if(hfont) { DeleteObject(hfont); hfont = nullptr; }
  if(hwnd) { DestroyWindow(hwnd); hwnd = nullptr; }
  if(auto parent = _parent()) {
    parent->setGeometry(parent->state().geometry);
  }
}

auto pStatusBar::setEnabled(bool enabled) -> void {
  //unsupported
}

auto pStatusBar::setFont(const string& font) -> void {
  if(hfont) DeleteObject(hfont);
  hfont = pFont::create(font);
  if(hwnd) SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

auto pStatusBar::setText(const string& text) -> void {
  if(hwnd) SendMessage(hwnd, SB_SETTEXT, 0, (LPARAM)(wchar_t*)utf16_t(text));
}

auto pStatusBar::setVisible(bool visible) -> void {
  if(hwnd) ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
  if(auto parent = _parent()) {
    parent->setGeometry(parent->state().geometry);
  }
}

auto pStatusBar::_parent() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow(true)) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
