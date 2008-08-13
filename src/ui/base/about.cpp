uintptr_t AboutWindow::close(event_t) {
  hide();
  return false;
}

void AboutWindow::setup() {
  create(Window::AutoCenter, 283, 76, translate["About bsnes ..."]);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  icon.create(0, 48, 48);
  about.create(0, 225, 48, string()
    << "bsnes -- " << translate["Version"] << " " << BSNES_VERSION "\n"
    << translate["Author"] << ": byuu\n"
    << translate["Project began: October 14th, 2004"]
  );
  localization.create(0, 273, 18, translate["Localization by: byuu"]);

  attach(icon, 5, 5);
  attach(about, 58, 5);
  attach(localization, 5, 58);

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
