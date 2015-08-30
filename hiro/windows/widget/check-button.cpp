#if defined(Hiro_CheckButton)

namespace hiro {

static auto CALLBACK CheckButton_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto button = dynamic_cast<mCheckButton*>(object)) {
      if(auto self = button->self()) {
        if(msg == WM_ERASEBKGND) return DefWindowProc(hwnd, msg, wparam, lparam);
        if(msg == WM_PAINT) return Button_paintProc(hwnd, msg, wparam, lparam,
          button->state.bordered, button->state.checked, button->enabled(true), button->font(true),
          button->state.image, button->state.orientation, button->state.text
        );
        return self->windowProc(hwnd, msg, wparam, lparam);
      }
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pCheckButton::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CheckButton_windowProc);
  pWidget::_setState();
  _setState();
  setChecked(state().checked);
}

auto pCheckButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pCheckButton::minimumSize() const -> Size {
  Size image = state().image.size();
  Size text = state().text ? pFont::size(self().font(true), state().text) : Size{};
  Size size;
  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(image.width() + (image && text ? 5 : 0) + text.width());
    size.setHeight(max(image.height(), text.height()));
  }
  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(image.width(), text.width()));
    size.setHeight(image.height() + (image && text ? 5 : 0) + text.height());
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

auto pCheckButton::setImage(const Image& image) -> void {
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

auto pCheckButton::onToggle() -> void {
  state().checked = !state().checked;
  setChecked(state().checked);
  self().doToggle();
}

auto pCheckButton::_setState() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
