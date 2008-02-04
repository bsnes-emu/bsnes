const char AboutWindow::about_text[4096] = ""
  "bsnes -- version " BSNES_VERSION "\n"
  "Author: byuu\n"
  "Project began: October 14th, 2004\n"
  "\n\n"
  "Contributors:\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\n"
  "   Overload, Richard Bannister, TRAC, zones";

uintptr_t AboutWindow::close(Event) {
  hide();
  return false;
}

void AboutWindow::setup() {
  create(Window::AutoCenter, 400, 200, "About bsnes ...");
  about.create(0, 390, 190, about_text);
  attach(about, 5, 5);

  on_close = bind(&AboutWindow::close, this);
}
