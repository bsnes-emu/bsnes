StateManager *stateManager = 0;

StateManager::StateManager() {
  setTitle("State Manager");
  setGeometry({ 128, 128, 600, 360 });
  windowManager->append(this, "StateManager");

  stateList.setHeaderText("Slot", "Description");
  stateList.setHeaderVisible();
  descLabel.setText("Description:");
  loadButton.setText("Load");
  saveButton.setText("Save");
  eraseButton.setText("Erase");

  append(layout);
  layout.setMargin(5);
  layout.append(stateList, ~0, ~0, 5);
  layout.append(descLayout, ~0, 0, 5);
    descLayout.append(descLabel, 0, 0, 5);
    descLayout.append(descEdit, ~0, 0);
  layout.append(controlLayout, ~0, 0);
    controlLayout.append(spacer, ~0, 0);
    controlLayout.append(loadButton, 80, 0, 5);
    controlLayout.append(saveButton, 80, 0, 5);
    controlLayout.append(eraseButton, 80, 0);

  for(unsigned n = 0; n < 32; n++) stateList.append(decimal<2>(n + 1), "(empty)");
  stateList.autoSizeColumns();

  synchronize();

  stateList.onActivate = { &StateManager::slotLoad, this };
  stateList.onChange = { &StateManager::synchronize, this };
  descEdit.onChange = { &StateManager::slotSaveDescription, this };
  loadButton.onTick = { &StateManager::slotLoad, this };
  saveButton.onTick = { &StateManager::slotSave, this };
  eraseButton.onTick = { &StateManager::slotErase, this };
}

void StateManager::synchronize() {
  layout.setEnabled(interface->cartridgeLoaded());

  descEdit.setText("");
  descEdit.setEnabled(false);
  controlLayout.setEnabled(stateList.selected());
  if(stateList.selected() == false) return;

  if(slot[stateList.selection()].capacity() > 0) {
    descEdit.setText(slotLoadDescription(stateList.selection()));
    descEdit.setEnabled(true);
  }
}

void StateManager::refresh() {
  for(unsigned n = 0; n < 32; n++) {
    stateList.modify(n, decimal<2>(n + 1), slotLoadDescription(n));
  }
  stateList.autoSizeColumns();
}

void StateManager::reset() {
  for(unsigned n = 0; n < 32; n++) slot[n] = serializer();
  synchronize();
  refresh();
}

bool StateManager::load(const string &filename, unsigned revision) {
  for(unsigned n = 0; n < 32; n++) slot[n] = serializer();
  synchronize();

  file fp;
  if(fp.open(filename, file::mode::read) == false) return false;

  if(fp.readl(4) == 0x31415342) {
    if(fp.readl(4) == revision) {  //'BSA1'
      for(unsigned n = 0; n < 32; n++) {
        if(fp.read() == false) continue;
        unsigned size = fp.readl(4);
        uint8_t *data = new uint8_t[size];
        fp.read(data, size);
        slot[n] = serializer(data, size);
        delete[] data;
      }
    }
  }

  refresh();
  return true;
}

bool StateManager::save(const string &filename, unsigned revision) {
  bool hasSave = false;
  for(unsigned n = 0; n < 32; n++) {
    if(slot[n].capacity() > 0) hasSave = true;
  }

  if(hasSave == false) {
    unlink(filename);
    return true;
  }

  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;

  fp.writel(0x31415342, 4);  //'BSA1'
  fp.writel(revision, 4);
  for(unsigned n = 0; n < 32; n++) {
    if(slot[n].capacity() == 0) {
      fp.write(false);
    } else {
      fp.write(true);
      fp.writel(slot[n].size(), 4);
      fp.write(slot[n].data(), slot[n].size());
    }
  }
}

void StateManager::slotLoad() {
  if(stateList.selected() == false) return;
  serializer s(slot[stateList.selection()].data(), slot[stateList.selection()].capacity());
  interface->unserialize(s);
}

void StateManager::slotSave() {
  if(stateList.selected()) {
    slot[stateList.selection()] = interface->serialize();
  }
  refresh();
  synchronize();
  descEdit.setFocused();
}

void StateManager::slotErase() {
  if(stateList.selected()) {
    slot[stateList.selection()] = serializer();
  }
  refresh();
  synchronize();
}

string StateManager::slotLoadDescription(unsigned n) {
  if(slot[n].capacity() == 0) return "(empty)";
  char text[DescriptionLength];
  strlcpy(text, (const char*)slot[n].data() + HeaderLength, DescriptionLength);
  return text;
}

void StateManager::slotSaveDescription() {
  if(stateList.selected() == false) return;
  string text = descEdit.text();
  if(slot[stateList.selection()].capacity() > 0) {
    strlcpy((char*)slot[stateList.selection()].data() + HeaderLength, (const char*)text, DescriptionLength);
  }
  refresh();
}
