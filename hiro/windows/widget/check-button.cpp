#if defined(Hiro_CheckButton)

namespace hiro {

auto pCheckButton::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  _setState();
  setChecked(state().checked);
}

auto pCheckButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pCheckButton::minimumSize() const -> Size {
  Size icon = {(int)state().icon.width(), (int)state().icon.height()};
  Size text = state().text ? pFont::size(self().font(true), state().text) : Size{};
  Size size;
  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(icon.width() + (icon && text ? 5 : 0) + text.width());
    size.setHeight(max(icon.height(), text.height()));
  }
  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(icon.width(), text.width()));
    size.setHeight(icon.height() + (icon && text ? 5 : 0) + text.height());
  }
  size.setHeight(max(size.height(), pFont::size(self().font(true), " ").height()));
  return {size.width() + (state().bordered && text ? 20 : 10), size.height() + 10};
}

auto pCheckButton::setBordered(bool bordered) -> void {
  _setState();
}

auto pCheckButton::setChecked(bool checked) -> void {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

auto pCheckButton::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  _setState();
}

auto pCheckButton::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  _setState();
}

auto pCheckButton::setIcon(const image& icon) -> void {
  _setState();
}

auto pCheckButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

auto pCheckButton::setText(const string& text) -> void {
  _setState();
}

auto pCheckButton::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  _setState();
}

//

auto pCheckButton::onToggle() -> void {
  state().checked = !state().checked;
  setChecked(state().checked);
  self().doToggle();
}

auto pCheckButton::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_PAINT) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    auto buttonState = Button_GetState(hwnd);
    Button_CustomDraw(hwnd, ps,
      state().bordered, state().checked, self().enabled(true), buttonState,
      self().font(true), state().icon, state().orientation, state().text
    );
    EndPaint(hwnd, &ps);
    return false;
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

//

auto pCheckButton::_setState() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
