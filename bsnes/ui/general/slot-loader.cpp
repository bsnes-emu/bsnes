SingleSlotLoader singleSlotLoader;
DoubleSlotLoader doubleSlotLoader;

void SingleSlotLoader::create() {
  application.addWindow(this, "SingleSlotLoader", "160,160");

  baseLabel.setText("Base:");
  baseBrowse.setText("...");
  slotLabel.setText("Slot:");
  slotBrowse.setText("...");
  okButton.setText("Ok");

  layout.setMargin(5);
  baseLayout.append(baseLabel, 40, 0, 5);
  baseLayout.append(basePath, 0, 0, 5);
  baseLayout.append(baseBrowse, Style::LineEditHeight, 0);
  layout.append(baseLayout, 0, Style::LineEditHeight, 5);
  slotLayout.append(slotLabel, 40, 0, 5);
  slotLayout.append(slotPath, 0, 0, 5);
  slotLayout.append(slotBrowse, Style::LineEditHeight, 0);
  layout.append(slotLayout, 0, Style::LineEditHeight, 5);
  controlLayout.append(spacer, 0, 0);
  controlLayout.append(okButton, 80, 0);
  layout.append(controlLayout, 0, Style::ButtonHeight);

  setGeometry({ 0, 0, 480, layout.minimumHeight() });
  append(layout);

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
  application.addWindow(this, "DoubleSlotLoader", "160,160");

  baseLabel.setText("Base:");
  baseBrowse.setText("...");
  slotALabel.setText("Slot A:");
  slotABrowse.setText("...");
  slotBLabel.setText("Slot B:");
  slotBBrowse.setText("...");
  okButton.setText("Ok");

  layout.setMargin(5);
  baseLayout.append(baseLabel, 40, 0, 5);
  baseLayout.append(basePath, 0, 0, 5);
  baseLayout.append(baseBrowse, Style::LineEditHeight, 0);
  layout.append(baseLayout, 0, Style::LineEditHeight, 5);
  slotALayout.append(slotALabel, 40, 0, 5);
  slotALayout.append(slotAPath, 0, 0, 5);
  slotALayout.append(slotABrowse, Style::LineEditHeight, 0);
  layout.append(slotALayout, 0, Style::LineEditHeight, 5);
  slotBLayout.append(slotBLabel, 40, 0, 5);
  slotBLayout.append(slotBPath, 0, 0, 5);
  slotBLayout.append(slotBBrowse, Style::LineEditHeight, 0);
  layout.append(slotBLayout, 0, Style::LineEditHeight, 5);
  controlLayout.append(spacer, 0, 0);
  controlLayout.append(okButton, 80, 0);
  layout.append(controlLayout, 0, Style::ButtonHeight);

  setGeometry({ 0, 0, 480, layout.minimumHeight() });
  append(layout);

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
