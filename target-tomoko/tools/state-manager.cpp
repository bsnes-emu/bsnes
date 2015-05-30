StateManager::StateManager(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Application::FileManager);
  setText("State Manager");

  layout.setMargin(5);
  stateList.append(ListViewColumn().setText("Slot").setForegroundColor({0, 128, 0}).setHorizontalAlignment(1.0));
  stateList.append(ListViewColumn().setText("Description").setWidth(~0));
  for(unsigned slot = 0; slot < Slots; slot++) {
    stateList.append(ListViewItem().setText(0, 1 + slot));
  }
  stateList.setHeaderVisible();
  stateList.onActivate([&] { doLoad(); });
  stateList.onChange([&] { doChange(); });
  descriptionLabel.setText("Description:");
  descriptionValue.onChange([&] { doLabel(); });
  saveButton.setText("Save").onActivate([&] { doSave(); });
  loadButton.setText("Load").onActivate([&] { doLoad(); });
  resetButton.setText("Reset").onActivate([&] { doReset(); });
  eraseButton.setText("Erase").onActivate([&] { doErase(); });
}

auto StateManager::doChange() -> void {
  vector<uint8> buffer;
  if(auto item = stateList.selected()) {
    buffer = file::read(program->stateName(1 + item->offset(), true));
  }

  if(buffer.size() >= 584) {
    string description;
    description.reserve(512);
    memory::copy(description.pointer(), buffer.data() + 72, 512);
    description.resize(description.length());
    descriptionValue.setEnabled(true).setText(description);
    loadButton.setEnabled(true);
    eraseButton.setEnabled(true);
  } else {
    descriptionValue.setEnabled(false).setText("");
    loadButton.setEnabled(false);
    eraseButton.setEnabled(false);
  }
}

auto StateManager::doRefresh() -> void {
  for(unsigned slot = 0; slot < Slots; slot++) {
    auto buffer = file::read(program->stateName(1 + slot, true));
    if(buffer.size() >= 584) {
      string description;
      description.reserve(512);
      memory::copy(description.pointer(), buffer.data() + 72, 512);
      description.resize(description.length());
      stateList.item(slot)->setText(1, description);
    } else {
      stateList.item(slot)->setText(1, "(empty)");
    }
  }
  doChange();
}

auto StateManager::doLabel() -> void {
  if(auto item = stateList.selected()) {
    auto buffer = file::read(program->stateName(1 + item->offset(), true));
    if(buffer.size() >= 584) {
      string description = descriptionValue.text();
      description.reserve(512);
      memory::copy(buffer.data() + 72, description.data(), 512);
      file::write(program->stateName(1 + item->offset(), true), buffer);
      doRefresh();
    }
  }
}

auto StateManager::doLoad() -> void {
  if(auto item = stateList.selected()) {
    program->loadState(1 + item->offset(), true);
  }
}

auto StateManager::doSave() -> void {
  if(auto item = stateList.selected()) {
    program->saveState(1 + item->offset(), true);
    doRefresh();
    descriptionValue.setFocused();
  }
}

auto StateManager::doReset() -> void {
  if(MessageDialog().setParent(*toolsManager).setText("Permanently erase all slots?").question() == "Yes") {
    for(auto slot : range(Slots)) file::remove(program->stateName(1 + slot, true));
    doRefresh();
  }
}

auto StateManager::doErase() -> void {
  if(auto item = stateList.selected()) {
    file::remove(program->stateName(1 + item->offset(), true));
    doRefresh();
  }
}
