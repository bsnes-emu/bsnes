Geometry pFont::geometry(const string &description, const string &text) {
  HFONT hfont = pFont::create(description);
  Geometry geometry = pFont::geometry(hfont, text);
  pFont::free(hfont);
  return geometry;
}

HFONT pFont::create(const string &description) {
  lstring part;
  part.split(",", description);
  foreach(item, part) item.trim(" ");

  string family = part[0];
  unsigned size = decimal(part[1]);
  bool bold = part[2].position("Bold");
  bool italic = part[2].position("Italic");
  if(family == "") family = "Tahoma";
  if(size == 0) size = 8;

  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),
    0, 0, 0, bold == false ? FW_NORMAL : FW_BOLD, italic, 0, 0, 0, 0, 0, 0, 0,
    utf16_t(family)
  );
}

void pFont::free(HFONT hfont) {
  DeleteObject(hfont);
}

Geometry pFont::geometry(HFONT hfont, const string &text_) {
  //temporary fix: empty text string returns height of zero; bad for eg Button height
  string text = (text_ == "" ? " " : text_);

  HDC hdc = GetDC(0);
  SelectObject(hdc, hfont);
  RECT rc = { 0, 0, 0, 0 };
  DrawText(hdc, utf16_t(text), -1, &rc, DT_CALCRECT);
  ReleaseDC(0, hdc);
  return { 0, 0, rc.right, rc.bottom };
}
