#if defined(Hiro_CheckButton)

namespace hiro {

auto pCheckButton::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  _setState();
  setBordered(state().bordered);
  setChecked(state().checked);
}

auto pCheckButton::destruct() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }
  DestroyWindow(hwnd);
}

auto pCheckButton::minimumSize() -> Size {
  auto size = pFont::size(hfont, state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width);
    size.setHeight(max(size.height(), state().icon.height));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width));
    size.setHeight(size.height() + state().icon.height);
  }

  return {size.width() + (state().text ? 20 : 10), size.height() + 10};
}

auto pCheckButton::setBordered(bool bordered) -> void {
}

auto pCheckButton::setChecked(bool checked) -> void {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
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

auto pCheckButton::onToggle() -> void {
  state().checked = !state().checked;
  setChecked(state().checked);
  self().doToggle();
}

auto pCheckButton::_setState() -> void {
  image icon = state().icon;
  icon.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }

  if(OsVersion() < WindowsVista) icon.alphaBlend(GetSysColor(COLOR_BTNFACE));

  hbitmap = CreateBitmap(icon);
  himagelist = ImageList_Create(icon.width, icon.height, ILC_COLOR32, 1, 0);
  ImageList_Add(himagelist, hbitmap, NULL);
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
