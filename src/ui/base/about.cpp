const char AboutWindow::about_text[1024] = ""
  "bsnes -- version " BSNES_VERSION "\n"
  "Author: byuu\n"
  "Project began: October 14th, 2004";

const char AboutWindow::contributors_text[1024] =
  "Contributors:\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\n"
  "   Overload, Richard Bannister, TRAC, zones";

uintptr_t AboutWindow::close(Event) {
  hide();
  return false;
}

void AboutWindow::setup() {
  create(Window::AutoCenter, 350, 125, "About bsnes ...");
  set_icon(48, 48, (uint32_t*)resource::icon48);

  icon.create(0, 48, 48);
  about.create(0, 287, 48, about_text);
  contributors.create(0, 340, 48, contributors_text);

  attach(icon, 5, 5);
  attach(about, 58, 5);
  attach(contributors, 5, 58);

  on_close = bind(&AboutWindow::close, this);

  uint8_t *buffer = new uint8_t[48 * 48 * 4];
  memcpy(buffer, resource::icon48, 48 * 48 * 4);
  for(unsigned i = 0; i < 48 * 48; i++) {
    uint8_t alpha = buffer[i * 4 + 3];
    buffer[i * 4 + 2] = uint8_t(1.0 / 256.0 * alpha * buffer[i * 4 + 2]);
    buffer[i * 4 + 1] = uint8_t(1.0 / 256.0 * alpha * buffer[i * 4 + 1]);
    buffer[i * 4 + 0] = uint8_t(1.0 / 256.0 * alpha * buffer[i * 4 + 0]);
  }
  memcpy(icon.buffer(), buffer, 48 * 48 * 4);
  delete[] buffer;
}
