namespace phoenix {

string pFont::serif(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Georgia, ", size, ", ", style};
}

string pFont::sans(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Tahoma, ", size, ", ", style};
}

string pFont::monospace(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Lucida Console, ", size, ", ", style};
}

Size pFont::size(const string &font, const string &text) {
  HFONT hfont = pFont::create(font);
  Size size = pFont::size(hfont, text);
  pFont::free(hfont);
  return size;
}

HFONT pFont::create(const string &description) {
  lstring part;
  part.split(",", description);
  for(auto &item : part) item.trim(" ");

  string family = "Sans";
  unsigned size = 8u;
  bool bold = false;
  bool italic = false;

  if(part[0] != "") family = part[0];
  if(part.size() >= 2) size = decimal(part[1]);
  if(part.size() >= 3) bold = part[2].position("Bold");
  if(part.size() >= 3) italic = part[2].position("Italic");

  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),
    0, 0, 0, bold == false ? FW_NORMAL : FW_BOLD, italic, 0, 0, 0, 0, 0, 0, 0,
    utf16_t(family)
  );
}

void pFont::free(HFONT hfont) {
  DeleteObject(hfont);
}

Size pFont::size(HFONT hfont, const string &text_) {
  //temporary fix: empty text string returns height of zero; bad for eg Button height
  string text = (text_ == "" ? " " : text_);

  HDC hdc = GetDC(0);
  SelectObject(hdc, hfont);
  RECT rc = {0, 0, 0, 0};
  DrawText(hdc, utf16_t(text), -1, &rc, DT_CALCRECT);
  ReleaseDC(0, hdc);
  return {rc.right, rc.bottom};
}

}
