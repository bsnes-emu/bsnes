#if defined(Hiro_Font)

namespace hiro {

auto pFont::size(const Font& font, const string& text) -> Size {
  auto hfont = pFont::create(font);
  auto size = pFont::size(hfont, text);
  pFont::free(hfont);
  return size;
}

auto pFont::size(HFONT hfont, const string& text) -> Size {
  HDC hdc = GetDC(0);
  SelectObject(hdc, hfont);
  RECT rc = {0, 0, 0, 0};
  DrawText(hdc, utf16_t(text), -1, &rc, DT_CALCRECT);
  ReleaseDC(0, hdc);
  return {rc.right, rc.bottom};
}

auto pFont::family(const string& family) -> string {
  if(family == Font::Sans ) return "Tahoma";
  if(family == Font::Serif) return "Georgia";
  if(family == Font::Mono ) return "Lucida Console";
  return family ? family : "Tahoma";
}

auto pFont::create(const Font& font) -> HFONT {
  static float dpi = Monitor::dpi().x();
  return CreateFont(
    -((font.size() ? font.size() : 8) * dpi / 72.0 + 0.5),
    0, 0, 0, font.bold() ? FW_BOLD : FW_NORMAL, font.italic(), 0, 0, 0, 0, 0, 0, 0,
    utf16_t(family(font.family()))
  );
}

auto pFont::free(HFONT hfont) -> void {
  DeleteObject(hfont);
}

}

#endif
