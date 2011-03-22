StateManager stateManager;

void StateManager::create() {
  setTitle("State Manager");
  application.addWindow(this, "StateManager", "160,160");

  stateList.setHeaderText("Slot", "Description");
  stateList.setHeaderVisible();
  descLabel.setText("Description:");
  loadButton.setText("Load");
  saveButton.setText("Save");
  eraseButton.setText("Erase");

  layout.setMargin(5);
  layout.append(stateList,          ~0, ~0, 5);
  descLayout.append(descLabel,      80,  0, 5);
  descLayout.append(descEdit,       ~0,  0   );
  layout.append(descLayout,                 5);
  controlLayout.append(spacer,       0,  0   );
  controlLayout.append(loadButton,  80,  0, 5);
  controlLayout.append(saveButton,  80,  0, 5);
  controlLayout.append(eraseButton, 80,  0   );
  layout.append(controlLayout                );

  setGeometry({ 0, 0, 480, layout.minimumGeometry().height + 250 });
  append(layout);

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
  if(stateList.selected() == false) return;
  if(slot[stateList.selection()].capacity() > 0) {
    descEdit.setText(slotLoadDescription(stateList.selection()));
    descEdit.setEnabled(true);
  }
}

void StateManager::refresh() {
  for(unsigned i = 0; i < 32; i++) {
    stateList.modify(i, rdecimal<2>(i + 1), slotLoadDescription(i));
  }
  stateList.autoSizeColumns();
}

void StateManager::load() {
  stateList.reset();
  for(unsigned i = 0; i < 32; i++) {
    slot[i] = serializer();
    stateList.append("");
  }

  file fp;
  if(fp.open(path.load(utility.slotPath(), ".bsa"), file::mode::read)) {
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
    unlink(path.load(utility.slotPath(), ".bsa"));
  } else {
    file fp;
    if(fp.open(path.load(utility.slotPath(), ".bsa"), file::mode::write)) {
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
  if(stateList.selected() == false) return;
  serializer s(slot[stateList.selection()].data(), slot[stateList.selection()].capacity());
  SNES::system.unserialize(s);
}

void StateManager::slotSave() {
  if(stateList.selected()) {
    SNES::system.runtosave();
    slot[stateList.selection()] = SNES::system.serialize();
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

string StateManager::slotLoadDescription(unsigned i) {
  if(slot[i].capacity() == 0) return "(empty)";
  char text[512];
  strlcpy(text, (const char*)slot[i].data() + HeaderLength, 512);
  return text;
}

void StateManager::slotSaveDescription() {
  if(stateList.selected() == false) return;
  string text = descEdit.text();
  if(slot[stateList.selection()].capacity() > 0) {
    strlcpy((char*)slot[stateList.selection()].data() + HeaderLength, (const char*)text, 512);
  }
  refresh();
}
