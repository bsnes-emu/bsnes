const char AboutWindow::about_text[4096] = ""
  "bsnes -- version " BSNES_VERSION "\r\n"
  "Author: byuu\r\n"
  "Project began: October 14th, 2004\r\n"
  "\r\n\r\n"
  "Contributors:\r\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\r\n"
  "   Overload, Richard Bannister, TRAC, zones";

bool AboutWindow::close() {
  hide();
  return false;
}

void AboutWindow::setup() {
  create("center", 300, 150, "About bsnes ...");

  attach(about.create("multiline|readonly", 290, 140, about_text), 5, 5);
}
