MainWindow mainWindow;

void MainWindow::create() {
  Window::create(256, 256, 595, 448, string(SNES::Info::Name, " v", SNES::Info::Version));
//setBackgroundColor(0, 0, 0);

  system.create(*this, "System");
  systemQuit.create(system, "Quit");
  systemQuit.onTick = []() { application.quit = true; };
  setMenuVisible(true);
  settings.create(*this, "Settings");
  tools.create(*this, "Tools");
  help.create(*this, "Help");

  viewport.create(*this, 0, 0, 595, 448);
  setStatusVisible(true);
  setVisible(true);
  onClose = []() { application.quit = true; return false; };
}
