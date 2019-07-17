#if defined(Hiro_MenuItem)

namespace hiro {

auto pMenuItem::construct() -> void {
  _createBitmap();
}

auto pMenuItem::destruct() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = nullptr; }
}

auto pMenuItem::setIcon(const image& icon) -> void {
  _createBitmap();
  _synchronize();
}

auto pMenuItem::setText(const string& text) -> void {
  _synchronize();
}

auto pMenuItem::onActivate() -> void {
  self().doActivate();
}

auto pMenuItem::_createBitmap() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = nullptr; }

  if(auto icon = state().icon) {
    icon.alphaBlend(GetSysColor(COLOR_MENU));  //Windows does not alpha blend menu icons properly (leaves black outline)
    icon.scale(GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK), Interpolation::Linear);
    hbitmap = CreateBitmap(icon);
  }
}

}

#endif
