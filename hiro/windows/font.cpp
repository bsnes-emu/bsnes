#if defined(Hiro_Font)

namespace hiro {

auto pFont::serif(unsigned size, string style) -> string {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Georgia, ", size, ", ", style};
}

auto pFont::sans(unsigned size, string style) -> string {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Tahoma, ", size, ", ", style};
}

auto pFont::monospace(unsigned size, string style) -> string {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Lucida Console, ", size, ", ", style};
}

auto pFont::size(const string& font, const string& text) -> Size {
  HFONT hfont = pFont::create(font);
  Size size = pFont::size(hfont, text);
  pFont::free(hfont);
  return size;
}

auto pFont::create(const string& description) -> HFONT {
  lstring part = description.split<2>(",").strip();

  string family = "Tahoma";
  unsigned size = 8u;
  bool bold = false;
  bool italic = false;

  if(part[0] != "") family = part[0];
  if(part.size() >= 2) size = decimal(part[1]);
  if(part.size() >= 3) bold = (bool)part[2].find("Bold");
  if(part.size() >= 3) italic = (bool)part[2].find("Italic");

  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),
    0, 0, 0, bold ? FW_BOLD : FW_NORMAL, italic, 0, 0, 0, 0, 0, 0, 0,
    utf16_t(family)
  );
}

auto pFont::free(HFONT hfont) -> void {
  DeleteObject(hfont);
}

auto pFont::size(HFONT hfont, string text) -> Size {
  //temporary fix: empty text string returns height of zero; bad for eg Button height
  if(text.empty()) text = " ";

  HDC hdc = GetDC(0);
  SelectObject(hdc, hfont);
  RECT rc = {0, 0, 0, 0};
  DrawText(hdc, utf16_t(text), -1, &rc, DT_CALCRECT);
  ReleaseDC(0, hdc);
  return {rc.right, rc.bottom};
}

}

#endif
