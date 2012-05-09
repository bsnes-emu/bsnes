StateManager *stateManager = nullptr;

StateManager::StateManager() {
  setGeometry({128, 128, 600, 360});
  windowManager->append(this, "StateManager");

  setTitle("State Manager");
  layout.setMargin(5);
  stateList.setHeaderText("Slot", "Description");
  stateList.setHeaderVisible();
  for(unsigned n = 0; n < Slots; n++) stateList.append(decimal<2>(1 + n), "(empty)");
  stateList.autoSizeColumns();
  descLabel.setText("Description:");
  saveButton.setText("Save");
  loadButton.setText("Load");
  resetButton.setText("Reset");
  eraseButton.setText("Erase");

  append(layout);
  layout.append(stateList, {~0, ~0}, 5);
  layout.append(descLayout, {~0, 0}, 5);
    descLayout.append(descLabel, {0, 0}, 5);
    descLayout.append(descEdit, {~0, 0});
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(saveButton, {80, 0}, 5);
    controlLayout.append(loadButton, {80, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(resetButton, {80, 0}, 5);
    controlLayout.append(eraseButton, {80, 0});

  stateList.onChange = {&StateManager::synchronize, this};
  stateList.onActivate = {&StateManager::slotLoad, this};
  descEdit.onChange = {&StateManager::slotSaveDescription, this};
  saveButton.onActivate = {&StateManager::slotSave, this};
  loadButton.onActivate = {&StateManager::slotLoad, this};
  resetButton.onActivate = [&] {
    if(MessageWindow::question(*this, "All states will be erased. Are you sure you want to do this?")
    == MessageWindow::Response::Yes) reset();
  };
  eraseButton.onActivate = {&StateManager::slotErase, this};

  stateList.setSelection(0);
  synchronize();
}

void StateManager::synchronize() {
  layout.setEnabled(application->active);

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
  for(unsigned n = 0; n < Slots; n++) {
    stateList.modify(n, decimal<2>(1 + n), slotLoadDescription(n));
  }
  stateList.autoSizeColumns();
}

void StateManager::reset() {
  for(auto &slot : this->slot) slot = serializer();
  synchronize();
  refresh();
}

bool StateManager::load(const string &filename, unsigned revision) {
  for(auto &slot : this->slot) slot = serializer();
  synchronize();

  file fp;
  if(fp.open(filename, file::mode::read) == false) return false;

  if(fp.readl(4) == 0x31415342 /* 'BSA1' */ && fp.readl(4) == revision) {
    for(auto &slot : this->slot) {
      if(fp.read() == false) continue;  //slot is empty
      unsigned size = fp.readl(4);
      uint8_t *data = new uint8_t[size];
      fp.read(data, size);
      slot = serializer(data, size);
      delete[] data;
    }
  }

  refresh();
  synchronize();
  return true;
}

bool StateManager::save(const string &filename, unsigned revision) {
  bool hasSave = false;
  for(auto &slot : this->slot) hasSave |= slot.capacity() > 0;
  if(hasSave == false) {
    file::remove(filename);
    return true;
  }

  directory::create(dir(filename));

  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;

  fp.writel(0x31415342, 4);  //'BSA1'
  fp.writel(revision, 4);
  for(auto &slot : this->slot) {
    fp.write(slot.capacity() > 0);
    if(slot.capacity()) {
      fp.writel(slot.capacity(), 4);
      fp.write(slot.data(), slot.capacity());
    }
  }
}

void StateManager::slotLoad() {
  if(stateList.selected() == false) return;
  serializer s(slot[stateList.selection()].data(), slot[stateList.selection()].capacity());
  system().unserialize(s);
}

void StateManager::slotSave() {
  if(stateList.selected()) {
    slot[stateList.selection()] = system().serialize();
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

string StateManager::slotLoadDescription(unsigned id) {
  if(slot[id].capacity() == 0) return "(empty)";
  char text[DescriptionLength];
  strmcpy(text, (const char*)slot[id].data() + HeaderLength, DescriptionLength);
  return text;
}

void StateManager::slotSaveDescription() {
  if(stateList.selected() == false) return;
  string text = descEdit.text();
  if(slot[stateList.selection()].capacity() > 0) {
    strmcpy((char*)slot[stateList.selection()].data() + HeaderLength, (const char*)text, DescriptionLength);
  }
  refresh();
}
