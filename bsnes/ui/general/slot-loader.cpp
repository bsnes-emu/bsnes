SingleSlotLoader singleSlotLoader;
DoubleSlotLoader doubleSlotLoader;

void SingleSlotLoader::create() {
  Window::create(0, 0, 256, 256);
  application.addWindow(this, "SingleSlotLoader", "160,160");

  unsigned x = 5, y = 5, height = Style::TextBoxHeight, width = 365 + height;

  baseLabel.setText("Base:");
  baseBrowse.setText("...");
  slotLabel.setText("Slot:");
  slotBrowse.setText("...");
  okButton.setText("Ok");

  layout.append(baseLabel, x, y, 50, height);
  layout.append(basePath, x + 50, y, 300, height);
  layout.append(baseBrowse, x + 355, y, height, height); y += height + 5;
  layout.append(slotLabel, x, y, 50, height);
  layout.append(slotPath, x + 50, y, 300, height);
  layout.append(slotBrowse, x + 355, y, height, height); y += height + 5;
  layout.append(okButton, x + width - 90, y, 80, Style::ButtonHeight); y += Style::ButtonHeight + 5;
  setGeometry(0, 0, width, y);
  setLayout(layout);

  baseBrowse.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::Cartridge, [](string filename) {
      singleSlotLoader.basePath.setText(filename);
    });
  };

  slotBrowse.onTick = []() {
    FileBrowser::Mode fileMode = (singleSlotLoader.mode == Mode::SuperGameBoy
    ? FileBrowser::Mode::GameBoy : FileBrowser::Mode::Satellaview);
    fileBrowser.fileOpen(fileMode, [](string filename) {
      singleSlotLoader.slotPath.setText(filename);
    });
  };

  okButton.onTick = { &SingleSlotLoader::load, this };
}

void SingleSlotLoader::loadCartridgeBsxSlotted() {
  mode = Mode::BsxSlotted;
  setTitle("Load BS-X Slotted Cartridge");
  basePath.setText("");
  slotPath.setText("");
  setVisible();
  okButton.setFocused();
}

void SingleSlotLoader::loadCartridgeBsx() {
  mode = Mode::Bsx;
  setTitle("Load BS-X Cartridge");
  basePath.setText(config.path.satellaviewBios);
  slotPath.setText("");
  setVisible();
  okButton.setFocused();
}

void SingleSlotLoader::loadCartridgeSuperGameBoy() {
  mode = Mode::SuperGameBoy;
  setTitle("Load Super Game Boy cartridge");
  basePath.setText(config.path.superGameBoyBios);
  slotPath.setText("");
  setVisible();
  okButton.setFocused();
}

void SingleSlotLoader::load() {
  setVisible(false);

  switch(mode) {
    case Mode::BsxSlotted: {
      cartridge.loadBsxSlotted(basePath.text(), slotPath.text());
      break;
    }
    case Mode::Bsx: {
      config.path.satellaviewBios = basePath.text();
      cartridge.loadBsx(basePath.text(), slotPath.text());
      break;
    }
    case Mode::SuperGameBoy: {
      config.path.superGameBoyBios = basePath.text();
      cartridge.loadSuperGameBoy(basePath.text(), slotPath.text());
      break;
    }
  }
}

//

void DoubleSlotLoader::create() {
  Window::create(0, 0, 256, 256);
  application.addWindow(this, "DoubleSlotLoader", "160,160");

  unsigned x = 5, y = 5, height = Style::TextBoxHeight, width = 365 + height;

  baseLabel.setText("Base:");
  baseBrowse.setText("...");
  slotALabel.setText("Slot A:");
  slotABrowse.setText("...");
  slotBLabel.setText("Slot B:");
  slotBBrowse.setText("...");
  okButton.setText("Ok");

  layout.append(baseLabel, x, y, 50, height);
  layout.append(basePath, x + 50, y, 300, height);
  layout.append(baseBrowse, x + 355, y, height, height); y += height + 5;
  layout.append(slotALabel, x, y, 50, height);
  layout.append(slotAPath, x + 50, y, 300, height);
  layout.append(slotABrowse, x + 355, y, height, height); y += height + 5;
  layout.append(slotBLabel, x, y, 50, height);
  layout.append(slotBPath, x + 50, y, 300, height);
  layout.append(slotBBrowse, x + 355, y, height, height); y += height + 5;
  layout.append(okButton, x + width - 90, y, 80, Style::ButtonHeight); y += Style::ButtonHeight + 5;
  setGeometry(0, 0, width, y);
  setLayout(layout);

  baseBrowse.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::Cartridge, [](string filename) {
      doubleSlotLoader.basePath.setText(filename);
    });
  };

  slotABrowse.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::SufamiTurbo, [](string filename) {
      doubleSlotLoader.slotAPath.setText(filename);
    });
  };

  slotBBrowse.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::SufamiTurbo, [](string filename) {
      doubleSlotLoader.slotBPath.setText(filename);
    });
  };

  okButton.onTick = { &DoubleSlotLoader::load, this };
}

void DoubleSlotLoader::loadCartridgeSufamiTurbo() {
  setTitle("Load Sufami Turbo Cartridge");
  basePath.setText(config.path.sufamiTurboBios);
  slotAPath.setText("");
  slotBPath.setText("");
  setVisible();
  okButton.setFocused();
}

void DoubleSlotLoader::load() {
  setVisible(false);
  config.path.sufamiTurboBios = basePath.text();
  cartridge.loadSufamiTurbo(basePath.text(), slotAPath.text(), slotBPath.text());
}
