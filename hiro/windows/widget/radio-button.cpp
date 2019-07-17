#if defined(Hiro_RadioButton)

namespace hiro {

auto pRadioButton::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setGroup(state().group);
  _setState();
}

auto pRadioButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pRadioButton::minimumSize() const -> Size {
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

auto pRadioButton::setBordered(bool bordered) -> void {
  _setState();
}

auto pRadioButton::setChecked() -> void {
  if(auto& group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioButton = dynamic_cast<mRadioButton*>(object.data())) {
          if(auto self = radioButton->self()) {
            SendMessage(self->hwnd, BM_SETCHECK, (WPARAM)(&self->reference == &reference), 0);
          }
        }
      }
    }
  }
}

auto pRadioButton::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  _setState();
}

auto pRadioButton::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  _setState();
}

auto pRadioButton::setGroup(sGroup group) -> void {
  bool first = true;
  if(auto& group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioButton = dynamic_cast<mRadioButton*>(object.data())) {
          if(auto self = radioButton->self()) {
            SendMessage(self->hwnd, BM_SETCHECK, (WPARAM)(radioButton->state.checked = first), 0);
            first = false;
          }
        }
      }
    }
  }
}

auto pRadioButton::setIcon(const image& icon) -> void {
  _setState();
}

auto pRadioButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

auto pRadioButton::setText(const string& text) -> void {
  _setState();
}

auto pRadioButton::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  _setState();
}

//

auto pRadioButton::onActivate() -> void {
  if(state().checked) return;
  self().setChecked();
  self().doActivate();
}

auto pRadioButton::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
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

auto pRadioButton::_setState() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
