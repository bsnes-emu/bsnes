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

  return {size.width + 20, size.height + 10};
}

void pButton::setImage(const image& image, Orientation orientation) {
  nall::image nallImage = image;
  nallImage.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(himagelist) { ImageList_Destroy(himagelist); himagelist = 0; }

  if(OsVersion() >= WindowsVista) {
    hbitmap = CreateBitmap(nallImage);
    SendMessage(hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap);
    switch(orientation) {
    case Orientation::Horizontal: SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~BS_TOP); break;
    case Orientation::Vertical:   SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_TOP); break;
    }
  } else {
    //Windows XP and earlier cannot display bitmaps and text at the same time with BM_SETIMAGE
    //Use BCM_SETIMAGELIST instead. It does not support alpha blending, so blend with button color
    //The XP theme and above use a gradient fade background, so it won't be a perfect match there
    nallImage.alphaBlend(GetSysColor(COLOR_BTNFACE));
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
  }

  setText(button.state.text);  //update text to display nicely with image (or lack thereof)
}

void pButton::setText(const string& text) {
  if(text.empty()) {
    //bitmaps will not show up if text is empty
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) |  BS_BITMAP);
  } else {
    //text will not show up if BS_BITMAP is set
    SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~BS_BITMAP);
  }

  if(OsVersion() >= WindowsVista && button.state.image.empty() == false && text.empty() == false) {
    //Vista+ does not add spacing between the icon and text; causing them to run into each other
    SetWindowText(hwnd, utf16_t(string{" ", text}));
  } else {
    SetWindowText(hwnd, utf16_t(text));
  }
}

void pButton::constructor() {
  hwnd = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&button);
  setDefaultFont();
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

}
