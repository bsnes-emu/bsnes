const char AboutWindow::about_text[4096] = ""
  "bsnes -- version " BSNES_VERSION "\n"
  "Author: byuu\n"
  "Project began: October 14th, 2004\n"
  "\n\n"
  "Contributors:\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\n"
  "   Overload, Richard Bannister, TRAC, zones";

void AboutWindow::setup() {
  create(ui::Window::Center, 400, 200, "About bsnes ...");

  about.create(*this, 0, 5, 5, 390, 190, about_text);
}

bool AboutWindow::message(uint id, uintptr_t param) {
  if(id == ui::Message::Close) {
    hide();
    return false;
  }

  return true;
}
