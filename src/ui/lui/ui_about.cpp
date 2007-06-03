const char AboutWindow::about_text[4096] = ""
  "bsnes -- version " BSNES_VERSION "\n"
  "Author: byuu\n"
  "Project began: October 14th, 2004\n"
  "\n\n"
  "Contributors:\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\n"
  "   Overload, Richard Bannister, TRAC, zones";

void AboutWindow::setup() {
  create(ui::Window::Center, 300, 130, "About bsnes ...");

  about.create(*this, 0, 5, 5, 290, 120, about_text);
}

int AboutWindow::message(uint id, void *param) {
  if(id == ui::Message::Close) {
    hide();
    return false;
  }

  return 0;
}
