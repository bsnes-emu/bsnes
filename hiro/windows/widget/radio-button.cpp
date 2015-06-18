#if defined(Hiro_RadioButton)

namespace hiro {

auto pRadioButton::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  _setState();
  setBordered(state().bordered);
  if(state().checked) setChecked();
}

auto pRadioButton::destruct() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }
  DestroyWindow(hwnd);
}

auto pRadioButton::minimumSize() -> Size {
  auto size = pFont::size(hfont, state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width());
    size.setHeight(max(size.height(), state().icon.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width()));
    size.setHeight(size.height() + state().icon.height());
  }

  return {size.width() + (state().text ? 20 : 10), size.height() + 10};
}

auto pRadioButton::setBordered(bool bordered) -> void {
}

auto pRadioButton::setChecked() -> void {
  if(auto group = self().group()) {
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

auto pRadioButton::setGroup(sGroup group) -> void {
}

auto pRadioButton::setIcon(const image& icon) -> void {
  _setState();
}

auto pRadioButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

void pRadioButton::setText(const string& text) {
  _setState();
}

void pRadioButton::onActivate() {
  if(state().checked) return;
  self().setChecked();
  self().doActivate();
}

auto pRadioButton::_setState() -> void {
  image icon = state().icon;
  icon.transform();

  if(hbitmap) { DeleteObject(hbitmap); hbitmap = nullptr; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = nullptr; }

  if(OsVersion() < WindowsVista) icon.alphaBlend(GetSysColor(COLOR_BTNFACE));

  hbitmap = CreateBitmap(icon);
  himagelist = ImageList_Create(icon.width(), icon.height(), ILC_COLOR32, 1, 0);
  ImageList_Add(himagelist, hbitmap, nullptr);
  BUTTON_IMAGELIST list;
  list.himl = himagelist;
  switch(state().orientation) {
  case Orientation::Horizontal: SetRect(&list.margin, 5, 0, 0, 0); list.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT; break;
  case Orientation::Vertical:   SetRect(&list.margin, 0, 5, 0, 0); list.uAlign = BUTTON_IMAGELIST_ALIGN_TOP;  break;
  }
  Button_SetImageList(hwnd, &list);

  if(auto text = state().text) {
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) &~ BS_BITMAP);
    SetWindowText(hwnd, utf16_t(text));
  } else {
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_BITMAP);
    SetWindowText(hwnd, L"");
  }
}

}

#endif
