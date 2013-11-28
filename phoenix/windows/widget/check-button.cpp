namespace phoenix {

Size pCheckButton::minimumSize() {
  Size size = pFont::size(hfont, checkButton.state.text);

  if(checkButton.state.orientation == Orientation::Horizontal) {
    size.width += checkButton.state.image.width;
    size.height = max(checkButton.state.image.height, size.height);
  }

  if(checkButton.state.orientation == Orientation::Vertical) {
    size.width = max(checkButton.state.image.width, size.width);
    size.height += checkButton.state.image.height;
  }

  return {size.width + 20, size.height + 10};
}

void pCheckButton::setChecked(bool checked) {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

void pCheckButton::setImage(const image& image, Orientation orientation) {
  nall::image nallImage = image;
  nallImage.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }

  if(OsVersion() < WindowsVista) nallImage.alphaBlend(GetSysColor(COLOR_BTNFACE));
  hbitmap = CreateBitmap(nallImage);
  himagelist = ImageList_Create(nallImage.width, nallImage.height, ILC_COLOR32, 1, 0);
  ImageList_Add(himagelist, hbitmap, NULL);
  BUTTON_IMAGELIST list;
  list.himl = himagelist;
  switch(orientation) {
  case Orientation::Horizontal: SetRect(&list.margin, 5, 0, 0, 0); list.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT; break;
  case Orientation::Vertical:   SetRect(&list.margin, 0, 5, 0, 0); list.uAlign = BUTTON_IMAGELIST_ALIGN_TOP;  break;
  }
  Button_SetImageList(hwnd, &list);

  setText(checkButton.state.text);
}

void pCheckButton::setText(string text) {
  if(text.empty()) {
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_BITMAP);
  } else {
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~BS_BITMAP);
  }

  SetWindowText(hwnd, utf16_t(text));
}

void pCheckButton::constructor() {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&checkButton);
  setDefaultFont();
  setChecked(checkButton.state.checked);
  setImage(checkButton.state.image, checkButton.state.orientation);
//setText(checkButton.state.text);
  synchronize();
}

void pCheckButton::destructor() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }
  DestroyWindow(hwnd);
}

void pCheckButton::orphan() {
  destructor();
  constructor();
}

void pCheckButton::onToggle() {
  checkButton.state.checked = !checkButton.state.checked;
  setChecked(checkButton.state.checked);
  if(checkButton.onToggle) checkButton.onToggle();
}

}
