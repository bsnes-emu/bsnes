#if defined(Hiro_RadioButton)

namespace hiro {

static auto CALLBACK RadioButton_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto button = dynamic_cast<mRadioButton*>(object)) {
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

auto pRadioButton::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)RadioButton_windowProc);
  pWidget::_setState();
  setGroup(state().group);
  _setState();
}

auto pRadioButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pRadioButton::minimumSize() const -> Size {
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

auto pRadioButton::setImage(const Image& image) -> void {
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

auto pRadioButton::onActivate() -> void {
  if(state().checked) return;
  self().setChecked();
  self().doActivate();
}

auto pRadioButton::_setState() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
