NSSDipWindow nssDipWindow;

void NSSDipWindow::create() {
  application.addWindow(this, "NSSDipWindow", "160,160");
  setTitle("NSS DIP Settings");

  for(unsigned n = 0; n < 16; n++) {
    dipName[n].setText("Unused");
    dipName[n].setVisible(false);
    layout.append(dipName[n], { 0, 0, 16, 16 });

    dipValue[n].setVisible(false);
    layout.append(dipValue[n], { 0, 0, 16, 16 });
  }

  loadButton.setText("Load Cartridge");
  layout.append(loadButton, { 0, 0, 16, 16 });

  append(layout);
  setResizable(false);
  setGeometry({ 0, 0, 400, 240 });

  onClose = loadButton.onTick = { &NSSDipWindow::assign, this };
}

void NSSDipWindow::select() {
  setVisible(false);

  for(unsigned n = 0; n < 16; n++) {
    dipName[n].setText({ "DIP #", 1 + n, ":" });
    dipName[n].setVisible(false);
    dipValue[n].reset();
    dipValue[n].setVisible(false);
  }

  unsigned dipCount = SNES::cartridge.information.nss.setting.size();

  for(unsigned n = 0; n < dipCount; n++) {
    dipName[n].setText({ "DIP #", 1 + n, ": ", SNES::cartridge.information.nss.setting[n] });
    for(unsigned z = 0; z < SNES::cartridge.information.nss.option[n].size(); z++) {
      lstring part;
      part.split<1>(":", SNES::cartridge.information.nss.option[n][z]);
      dipValue[n].append(part[1]);
    }
  }

  unsigned maximumLabelWidth =  50;
  unsigned maximumComboWidth = 100;
  unsigned controlHeight = dipValue[0].minimumGeometry().height;

  for(unsigned n = 0; n < dipCount; n++) {
    maximumLabelWidth = max(maximumLabelWidth, dipName[n].minimumGeometry().width);
    maximumComboWidth = max(maximumComboWidth, dipValue[n].minimumGeometry().width);
  }

  for(unsigned n = 0; n < dipCount; n++) {
    dipName[n].setGeometry({ 5, 5 + (controlHeight + 5) * n, maximumLabelWidth, controlHeight });
    dipName[n].setVisible(true);

    dipValue[n].setGeometry({ 5 + maximumLabelWidth + 5, 5 + (controlHeight + 5) * n, maximumComboWidth, controlHeight });
    dipValue[n].setVisible(true);
  }

  unsigned buttonWidth  = loadButton.minimumGeometry().width;
  unsigned buttonHeight = loadButton.minimumGeometry().height;

  unsigned windowWidth  = 5 + maximumLabelWidth + 5 + maximumComboWidth + 5;
  unsigned windowHeight = 5 + (controlHeight + 5) * dipCount + buttonHeight + 5;

  loadButton.setGeometry({ windowWidth - 5 - buttonWidth, windowHeight - 5 - buttonHeight, buttonWidth, buttonHeight });

  setGeometry({ geometry().x, geometry().y, windowWidth, windowHeight });
  setVisible(true);
  loadButton.setFocused();
}

void NSSDipWindow::assign() {
  unsigned dip = 0;

  for(unsigned n = 0; n < SNES::cartridge.information.nss.setting.size(); n++) {
    unsigned position = dipValue[n].selection();
    lstring part;
    part.split<1>(":", SNES::cartridge.information.nss.option[n][position]);
    dip |= hex(part[0]);
  }

  SNES::nss.set_dip(dip);

  setVisible(false);
  application.pause = false;
}
