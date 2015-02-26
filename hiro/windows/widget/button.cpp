namespace phoenix {

#ifndef Button_SetImageList
  //MinGW/32-bit has painfully outdated platform headers ...
  typedef struct {
    HIMAGELIST himl;
    RECT margin;
    UINT uAlign;
  } BUTTON_IMAGELIST, *PBUTTON_IMAGELIST;

  #define BUTTON_IMAGELIST_ALIGN_LEFT 0
  #define BUTTON_IMAGELIST_ALIGN_RIGHT 1
  #define BUTTON_IMAGELIST_ALIGN_TOP 2
  #define BUTTON_IMAGELIST_ALIGN_BOTTOM 3
  #define BUTTON_IMAGELIST_ALIGN_CENTER 4

  #define BCM_FIRST 0x1600
  #define BCM_SETIMAGELIST (BCM_FIRST+2)
  #define Button_SetImageList(hwnd, pbuttonImagelist) (WINBOOL)SNDMSG((hwnd),BCM_SETIMAGELIST,0,(LPARAM)(pbuttonImagelist))
#endif

Size pButton::minimumSize() {
  Size size = pFont::size(hfont, button.state.text);

  if(button.state.orientation == Orientation::Horizontal) {
    size.width += button.state.image.width;
    size.height = max(button.state.image.height, size.height);
  }

  if(button.state.orientation == Orientation::Vertical) {
    size.width = max(button.state.image.width, size.width);
    size.height += button.state.image.height;
  }

  return {size.width + (button.state.text ? 20 : 10), size.height + 10};
}

void pButton::setBordered(bool bordered) {
}

void pButton::setImage(const image& image, Orientation orientation) {
  nall::image nallImage = image;
  nallImage.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }

  //Windows XP and earlier do not support translucent images
  //so we must blend with the button background color (which does not look great with XP gradient-button themes)
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

  setText(button.state.text);  //update text to display nicely with image (or lack thereof)
}

void pButton::setText(string text) {
  if(text.empty()) {
    //bitmaps will not show up if text is empty
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_BITMAP);
  } else {
    //text will not show up if BS_BITMAP is set
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~BS_BITMAP);
  }

  SetWindowText(hwnd, utf16_t(text));
}

void pButton::constructor() {
  hwnd = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&button);
  setDefaultFont();
  setBordered(button.state.bordered);
  setImage(button.state.image, button.state.orientation);
//setText(button.state.text);  //called by setImage();
  synchronize();
}

void pButton::destructor() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }
  DestroyWindow(hwnd);
}

void pButton::orphan() {
  destructor();
  constructor();
}

void pButton::onActivate() {
  if(button.onActivate) button.onActivate();
}

}
