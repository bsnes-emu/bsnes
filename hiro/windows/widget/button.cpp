#if defined(Hiro_Button)

namespace hiro {

auto pButton::construct() -> void {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  _setState();
  setBordered(state().bordered);
}

auto pButton::destruct() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }
  DestroyWindow(hwnd);
}

auto pButton::minimumSize() const -> Size {
  auto size = pFont::size(hfont, state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width());
    size.setHeight(max(size.height(), state().icon.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width()));
    size.setHeight(size.height() + state().icon.height());
  }

  return {size.width() + (state().text ? 20 : 13), size.height() + 10};
}

auto pButton::setBordered(bool bordered) -> void {
}

auto pButton::setIcon(const image& icon) -> void {
  _setState();
}

auto pButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

auto pButton::setText(const string& text) -> void {
  _setState();
}

auto pButton::onActivate() -> void {
  self().doActivate();
}

//performs setIcon, setOrientation, setText
auto pButton::_setState() -> void {
  image icon = state().icon;
  icon.transform();

  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }

  //Windows XP and earlier do not support translucent images
  //so we must blend with the button background color (which does not look great with XP gradient-button themes)
  if(OsVersion() < WindowsVista) icon.alphaBlend(GetSysColor(COLOR_BTNFACE));

  hbitmap = CreateBitmap(icon);
  himagelist = ImageList_Create(icon.width(), icon.height(), ILC_COLOR32, 1, 0);
  ImageList_Add(himagelist, hbitmap, NULL);
  BUTTON_IMAGELIST list;
  list.himl = himagelist;
  switch(state().orientation) {
  case Orientation::Horizontal: SetRect(&list.margin, 5, 0, 0, 0); list.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT; break;
  case Orientation::Vertical:   SetRect(&list.margin, 0, 5, 0, 0); list.uAlign = BUTTON_IMAGELIST_ALIGN_TOP;  break;
  }
  Button_SetImageList(hwnd, &list);

  if(auto text = state().text) {
    //text will not show up if BS_BITMAP is set
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) &~ BS_BITMAP);
    SetWindowText(hwnd, utf16_t(text));
  } else {
    //bitmaps will not show up if text is empty
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_BITMAP);
    SetWindowText(hwnd, L"");
  }
}

}

#endif
