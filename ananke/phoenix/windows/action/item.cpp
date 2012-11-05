void pItem::setImage(const image &image) {
  createBitmap();
  if(parentWindow) parentWindow->p.updateMenu();
}

void pItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pItem::constructor() {
  createBitmap();
}

void pItem::destructor() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(parentMenu) parentMenu->remove(item);
}

void pItem::createBitmap() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }

  if(item.state.image.width && item.state.image.height) {
    nall::image nallImage = item.state.image;
    nallImage.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    nallImage.alphaBlend(GetSysColor(COLOR_MENU));  //Windows does not alpha blend menu icons properly (leaves black outline)
    nallImage.scale(GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK), Interpolation::Linear);
    hbitmap = CreateBitmap(nallImage);
  }
}
