SlotLoader *slotLoader = 0;

SlotLoaderPath::SlotLoaderPath() {
  browse.setText("Browse ...");
  append(label,  { 40, 0 }, 5);
  append(path,   { ~0, 0 }, 5);
  append(browse, { 80, 0 }, 0);
}

SlotLoader::SlotLoader() {
  layout.setMargin(5);
  base.label.setText("Base:");
  slot[0].label.setText("Slot:");
  slot[1].label.setText("Slot:");
  loadButton.setText("Load");

  append(layout);
  layout.append(base,                { ~0, 0 }, 5);
  layout.append(slot[0],             { ~0, 0 }, 5);
  layout.append(slot[1],             { ~0, 0 }, 5);
  layout.append(controlLayout,       { ~0, 0 }, 0);
    controlLayout.append(spacer,     { ~0, 0 }, 0);
    controlLayout.append(loadButton, { 80, 0 }, 0);

  setGeometry({ 128, 128, 500, layout.minimumGeometry().height });
  windowManager->append(this, "SlotLoader");
}

void SlotLoader::synchronize() {
  loadButton.setEnabled(base.path.text() != "");
}

void SlotLoader::loadSatellaviewSlotted() {
  setTitle("Load Cartridge - Satellaview-Slotted");

  base.path.setText("");

  slot[0].path.setText("");
  slot[0].path.setEnabled(true);
  slot[0].browse.setEnabled(true);

  slot[1].path.setText("");
  slot[1].path.setEnabled(false);
  slot[1].browse.setEnabled(false);

  base.browse.onActivate = [&] {
    fileBrowser->open("Load Cartridge - SNES", FileBrowser::Mode::SNES, [&](const string &filename) {
      base.path.setText(filename);
      synchronize();
    });
  };

  slot[0].browse.onActivate = [&] {
    fileBrowser->open("Load Cartridge - Satellaview", FileBrowser::Mode::Satellaview, [&](const string &filename) {
      slot[0].path.setText(filename);
      synchronize();
    });
  };

  loadButton.onActivate = [&] {
    this->setVisible(false);
    interface->snes.loadSatellaviewSlottedCartridge(base.path.text(), slot[0].path.text());
  };

  synchronize();
  setVisible();
}

void SlotLoader::loadSatellaview() {
  setTitle("Load Cartridge - Satellaview");

  base.path.setText(config->path.bios.satellaview);

  slot[0].path.setText("");
  slot[0].path.setEnabled(true);
  slot[0].browse.setEnabled(true);

  slot[1].path.setText("");
  slot[1].path.setEnabled(false);
  slot[1].browse.setEnabled(false);

  base.browse.onActivate = [&] {
    fileBrowser->open("Load BIOS - Satellaview", FileBrowser::Mode::SNES, [&](const string &filename) {
      config->path.bios.satellaview = filename;
      base.path.setText(filename);
      synchronize();
    });
  };

  slot[0].browse.onActivate = [&] {
    fileBrowser->open("Load Cartridge - Satellaview", FileBrowser::Mode::Satellaview, [&](const string &filename) {
      slot[0].path.setText(filename);
      synchronize();
    });
  };

  loadButton.onActivate = [&] {
    this->setVisible(false);
    interface->snes.loadSatellaviewCartridge(base.path.text(), slot[0].path.text());
  };

  synchronize();
  setVisible();
}

void SlotLoader::loadSufamiTurbo() {
  setTitle("Load Cartridge - Sufami Turbo");

  base.path.setText(config->path.bios.sufamiTurbo);

  slot[0].path.setText("");
  slot[0].path.setEnabled(true);
  slot[0].browse.setEnabled(true);

  slot[1].path.setText("");
  slot[1].path.setEnabled(true);
  slot[1].browse.setEnabled(true);

  base.browse.onActivate = [&] {
    fileBrowser->open("Load BIOS - Sufami Turbo", FileBrowser::Mode::SNES, [&](const string &filename) {
      config->path.bios.sufamiTurbo = filename;
      base.path.setText(filename);
      synchronize();
    });
  };

  slot[0].browse.onActivate = [&] {
    fileBrowser->open("Load Cartridge - Sufami Turbo", FileBrowser::Mode::SufamiTurbo, [&](const string &filename) {
      slot[0].path.setText(filename);
      synchronize();
    });
  };

  slot[1].browse.onActivate = [&] {
    fileBrowser->open("Load Cartridge - Sufami Turbo", FileBrowser::Mode::SufamiTurbo, [&](const string &filename) {
      slot[1].path.setText(filename);
      synchronize();
    });
  };

  loadButton.onActivate = [&] {
    this->setVisible(false);
    interface->snes.loadSufamiTurboCartridge(base.path.text(), slot[0].path.text(), slot[1].path.text());
  };

  synchronize();
  setVisible();
}

void SlotLoader::loadSuperGameBoy() {
  setTitle("Load Cartridge - Super Game Boy");

  base.path.setText(config->path.bios.superGameBoy);

  slot[0].path.setText("");
  slot[0].path.setEnabled(true);
  slot[0].browse.setEnabled(true);

  slot[1].path.setText("");
  slot[1].path.setEnabled(false);
  slot[1].browse.setEnabled(false);

  base.browse.onActivate = [&] {
    fileBrowser->open("Load BIOS - Super Game Boy", FileBrowser::Mode::SNES, [&](const string &filename) {
      config->path.bios.superGameBoy = filename;
      base.path.setText(filename);
      synchronize();
    });
  };

  slot[0].browse.onActivate = [&] {
    fileBrowser->open("Load Cartridge - Game Boy", FileBrowser::Mode::GameBoy, [&](const string &filename) {
      slot[0].path.setText(filename);
      synchronize();
    });
  };

  loadButton.onActivate = [&] {
    this->setVisible(false);
    interface->snes.loadSuperGameBoyCartridge(base.path.text(), slot[0].path.text());
  };

  synchronize();
  setVisible();
}
