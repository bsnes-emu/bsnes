#if defined(Hiro_CheckLabel)

namespace hiro {

auto pCheckLabel::construct() -> void {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  setChecked(state().checked);
  setText(state().text);
}

auto pCheckLabel::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pCheckLabel::minimumSize() const -> Size {
  auto size = pFont::size(hfont, state().text);
  return {size.width() + 20, size.height() + 4};
}

auto pCheckLabel::setChecked(bool checked) -> void {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

auto pCheckLabel::setText(const string& text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pCheckLabel::onToggle() -> void {
  state().checked = !state().checked;
  setChecked(state().checked);
  self().doToggle();
}

}

#endif
