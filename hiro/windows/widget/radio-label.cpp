#if defined(Hiro_RadioLabel)

namespace hiro {

auto pRadioLabel::construct() -> void {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  if(state().checked) setChecked();
  setText(state().text);
}

auto pRadioLabel::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pRadioLabel::minimumSize() -> Size {
  auto size = pFont::size(hfont, state().text);
  return {size.width() + 20, size.height() + 4};
}

auto pRadioLabel::setChecked() -> void {
  if(auto group = self().group()) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioLabel = dynamic_cast<mRadioLabel*>(object.data())) {
          if(auto self = radioLabel->self()) {
            SendMessage(self->hwnd, BM_SETCHECK, (WPARAM)(&self->reference == &reference), 0);
          }
        }
      }
    }
  }
}

auto pRadioLabel::setGroup(sGroup group) -> void {
}

auto pRadioLabel::setText(const string& text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pRadioLabel::onActivate() -> void {
  if(state().checked) return;
  self().setChecked();
  self().doActivate();
}

}

#endif
