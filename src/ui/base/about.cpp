uintptr_t AboutWindow::close(Event) {
  hide();
  return false;
}

void AboutWindow::setup() {
  create(Window::AutoCenter, 320, 130, translate["About bsnes ..."]);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  icon.create(0, 48, 48);
  about.create(0, 225, 48, string()
    << "bsnes -- " << translate["Version"] << " " << BSNES_VERSION "\n"
    << translate["Author"] << ": byuu\n"
    << translate["Project began: October 14th, 2004"]
  );
  contributors.create(0, 275, 70, string()
    << translate["Contributors:"] << "\n"
    << "   anomie, blargg, DMV27, GIGO, kode54, Nach,\n"
    << "   Overload, Richard Bannister, TRAC, zones\n"
    << "\n"
    << translate["Localization by: byuu"]
  );

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
