namespace phoenix {

Size pRadioButton::minimumSize() {
  Size size = pFont::size(hfont, radioButton.state.text);

  if(radioButton.state.orientation == Orientation::Horizontal) {
    size.width += radioButton.state.image.width;
    size.height = max(radioButton.state.image.height, size.height);
  }

  if(radioButton.state.orientation == Orientation::Vertical) {
    size.width = max(radioButton.state.image.width, size.width);
    size.height += radioButton.state.image.height;
  }

  return {size.width + 20, size.height + 10};
}

void pRadioButton::setChecked() {
  for(auto& item : radioButton.state.group) {
    SendMessage(item.p.hwnd, BM_SETCHECK, (WPARAM)(&item == &radioButton), 0);
  }
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
}

void pRadioButton::setImage(const image& image, Orientation orientation) {
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

  setText(radioButton.state.text);
}

void pRadioButton::setText(string text) {
  if(text.empty()) {
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_BITMAP);
  } else {
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~BS_BITMAP);
  }

  SetWindowText(hwnd, utf16_t(text));
}

void pRadioButton::constructor() {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX | BS_PUSHLIKE,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&radioButton);
  setDefaultFont();
  if(radioButton.state.checked) setChecked();
  setImage(radioButton.state.image, radioButton.state.orientation);
//setText(radioButton.state.text);
  synchronize();
}

void pRadioButton::destructor() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }
  DestroyWindow(hwnd);
}

void pRadioButton::orphan() {
  destructor();
  constructor();
}

void pRadioButton::onActivate() {
  if(radioButton.state.checked) return;
  radioButton.setChecked();
  if(radioButton.onActivate) radioButton.onActivate();
}

}
