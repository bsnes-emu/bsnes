StateManager stateManager;

void StateManager::create() {
  Window::create(0, 0, 256, 256, "State Manager");
  application.addWindow(this, "StateManager", "160,160");

  unsigned x = 5, y = 5;

  stateList.create(*this, x, y, 500, 250, "Slot\tDescription"); y += 255;
  stateList.setHeaderVisible();

  descLabel.create(*this, x, y, 80, Style::TextBoxHeight, "Description:");
  descEdit.create(*this, x + 80, y, 420, Style::TextBoxHeight); y += Style::TextBoxHeight + 5;

  loadButton.create(*this, x + 505 - 85 - 85 - 85, y, 80, Style::ButtonHeight, "Load");
  saveButton.create(*this, x + 505 - 85 - 85, y, 80, Style::ButtonHeight, "Save");
  eraseButton.create(*this, x + 505 - 85, y, 80, Style::ButtonHeight, "Erase"); y += Style::ButtonHeight + 5;

  setGeometry(0, 0, 510, y);
  synchronize();

  stateList.onActivate = { &StateManager::slotLoad, this };
  stateList.onChange = { &StateManager::synchronize, this };
  descEdit.onChange = { &StateManager::slotSaveDescription, this };
  loadButton.onTick = { &StateManager::slotLoad, this };
  saveButton.onTick = { &StateManager::slotSave, this };
  eraseButton.onTick = { &StateManager::slotErase, this };
}

void StateManager::synchronize() {
  descEdit.setText("");
  descEdit.setEnabled(false);
  if(auto position = stateList.selection()) {
    if(slot[position()].capacity() > 0) {
      descEdit.setText(slotLoadDescription(position()));
      descEdit.setEnabled(true);
    }
  }
}

void StateManager::refresh() {
  for(unsigned i = 0; i < 32; i++) {
    stateList.setItem(i, { 
      strunsigned<2, ' '>(i + 1), "\t",
      slotLoadDescription(i)
    });
  }
  stateList.resizeColumnsToContent();
}

void StateManager::load() {
  stateList.reset();
  for(unsigned i = 0; i < 32; i++) {
    slot[i] = serializer();
    stateList.addItem("");
  }

  string filename = { cartridge.baseName, ".bsa" };
  file fp;
  if(fp.open(string(cartridge.baseName, ".bsa"), file::mode::read)) {
    if(fp.readl(4) == 0x31415342) {
      if(fp.readl(4) == SNES::Info::SerializerVersion) {
        for(unsigned i = 0; i < 32; i++) {
          if(fp.read() == false) continue;
          uint8_t *data = new uint8_t[SNES::system.serialize_size()];
          fp.read(data, SNES::system.serialize_size());
          slot[i] = serializer(data, SNES::system.serialize_size());
          delete[] data;
        }
      }
    }
  }

  refresh();
}

void StateManager::save() {
  bool hasSave = false;
  for(unsigned i = 0; i < 32; i++) {
    if(slot[i].capacity() > 0) hasSave = true;
  }

  if(hasSave == false) {
    unlink(string(cartridge.baseName, ".bsa"));
  } else {
    file fp;
    if(fp.open(string(cartridge.baseName, ".bsa"), file::mode::write)) {
      fp.writel(0x31415342, 4);  //'BSA1'
      fp.writel(SNES::Info::SerializerVersion, 4);

      for(unsigned i = 0; i < 32; i++) {
        if(slot[i].capacity() == 0) {
          fp.write(false);
        } else {
          fp.write(true);
          fp.write(slot[i].data(), slot[i].capacity());
        }
      }
    }
  }
}

void StateManager::slotLoad() {
  if(auto position = stateList.selection()) {
    serializer s(slot[position()].data(), slot[position()].capacity());
    SNES::system.unserialize(s);
  }
}

void StateManager::slotSave() {
  if(auto position = stateList.selection()) {
    SNES::system.runtosave();
    slot[position()] = SNES::system.serialize();
  }
  refresh();
  synchronize();
  descEdit.setFocused();
}

void StateManager::slotErase() {
  if(auto position = stateList.selection()) {
    slot[position()] = serializer();
  }
  refresh();
  synchronize();
}

string StateManager::slotLoadDescription(unsigned i) {
  if(slot[i].capacity() == 0) return "(empty)";
  char text[512];
  strlcpy(text, (const char*)slot[i].data() + HeaderLength, 512);
  return text;
}

void StateManager::slotSaveDescription() {
  if(auto position = stateList.selection()) {
    string text = descEdit.text();
    if(slot[position()].capacity() > 0) {
      strlcpy((char*)slot[position()].data() + HeaderLength, (const char*)text, 512);
    }
  }
  refresh();
}
