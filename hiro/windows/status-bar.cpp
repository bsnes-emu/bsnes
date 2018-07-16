#if defined(Hiro_StatusBar)

namespace hiro {

auto pStatusBar::construct() -> void {
  if(auto parent = _parent()) {
    hwnd = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD, 0, 0, 0, 0, parent->hwnd, nullptr, GetModuleHandle(0), 0);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
    setEnabled(self().enabled());
    setFont(self().font());
    setText(self().text());
    setVisible(self().visible());
  }
}

auto pStatusBar::destruct() -> void {
  if(hfont) { DeleteObject(hfont); hfont = nullptr; }
  if(hwnd) { DestroyWindow(hwnd); hwnd = nullptr; }

  if(auto parent = _parent()) {
    parent->setGeometry(parent->state().geometry);
  }
}

auto pStatusBar::setEnabled(bool) -> void {
  //unsupported
}

auto pStatusBar::setFont(const Font&) -> void {
  auto font = self().font(true);
  if(hfont) DeleteObject(hfont);
  hfont = pFont::create(font);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);

  auto& text = state().text;
  auto height = font.size(text ? text : " ").height();
  SendMessage(hwnd, SB_SETMINHEIGHT, (WPARAM)height, 0);

  if(auto parent = _parent()) {
    parent->setGeometry(parent->state().geometry);
  }
}

auto pStatusBar::setText(const string&) -> void {
  auto& text = state().text;
  SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)(wchar_t*)utf16_t(text));
}

auto pStatusBar::setVisible(bool) -> void {
  ShowWindow(hwnd, self().visible() ? SW_SHOWNORMAL : SW_HIDE);

  if(auto parent = _parent()) {
    parent->setGeometry(parent->state().geometry);
  }
}

auto pStatusBar::_parent() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow(true)) {
    if(auto self = parent->self()) return *self;
  }
  return {};
}

}

#endif
