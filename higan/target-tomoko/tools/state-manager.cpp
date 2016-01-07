StateManager::StateManager(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Application::FileManager);
  setText("State Manager");

  layout.setMargin(5);
  stateList.append(ListViewHeader().setVisible()
    .append(ListViewColumn().setText("Slot").setForegroundColor({0, 128, 0}).setAlignment(1.0))
    .append(ListViewColumn().setText("Description").setExpandable())
  );
  for(auto slot : range(Slots)) {
    stateList.append(ListViewItem()
      .append(ListViewCell().setText(1 + slot))
      .append(ListViewCell())
    );
  }
  stateList.onActivate([&] { doLoad(); });
  stateList.onChange([&] { doChangeSelected(); });
  descriptionLabel.setText("Description:");
  descriptionValue.onChange([&] { doChangeDescription(); });
  saveButton.setText("Save").onActivate([&] { doSave(); });
  loadButton.setText("Load").onActivate([&] { doLoad(); });
  resetButton.setText("Reset").onActivate([&] { doReset(); });
  eraseButton.setText("Erase").onActivate([&] { doErase(); });

  doUpdateControls();
}

auto StateManager::doUpdateControls() -> void {
  vector<uint8> buffer;
  if(auto item = stateList.selected()) {
    buffer = file::read(program->stateName(1 + item.offset(), true));
  }

  if(buffer.size() >= 584) {
    descriptionValue.setEnabled(true);
    loadButton.setEnabled(true);
    eraseButton.setEnabled(true);
  } else {
    descriptionValue.setEnabled(false).setText("");
    loadButton.setEnabled(false);
    eraseButton.setEnabled(false);
  }
}

auto StateManager::doChangeSelected() -> void {
  vector<uint8> buffer;
  if(auto item = stateList.selected()) {
    buffer = file::read(program->stateName(1 + item.offset(), true));
    if(buffer.size() >= 584) {
      string description;
      description.reserve(512);
      memory::copy(description.get(), buffer.data() + 72, 512);
      description.resize(description.length());
      descriptionValue.setEnabled(true).setText(description);
      return doUpdateControls();
    }
  }

  descriptionValue.setEnabled(false).setText("");
  doUpdateControls();
}

auto StateManager::doRefresh() -> void {
  for(auto slot : range(Slots)) {
    auto buffer = file::read(program->stateName(1 + slot, true));
    if(buffer.size() >= 584) {
      string description;
      description.reserve(512);
      memory::copy(description.get(), buffer.data() + 72, 512);
      description.resize(description.length());
      stateList.item(slot).cell(1).setText(description).setForegroundColor({0, 0, 0});
    } else {
      stateList.item(slot).cell(1).setText("(empty)").setForegroundColor({128, 128, 128});
    }
  }
}

auto StateManager::doChangeDescription() -> void {
  if(auto item = stateList.selected()) {
    auto buffer = file::read(program->stateName(1 + item.offset(), true));
    if(buffer.size() >= 584) {
      string description = descriptionValue.text();
      description.reserve(512);
      memory::copy(buffer.data() + 72, description.data(), 512);
      file::write(program->stateName(1 + item.offset(), true), buffer);
      doRefresh();
      doUpdateControls();
    }
  }
}

auto StateManager::doLoad() -> void {
  if(auto item = stateList.selected()) {
    program->loadState(1 + item.offset(), true);
  }
}

auto StateManager::doSave() -> void {
  if(auto item = stateList.selected()) {
    program->saveState(1 + item.offset(), true);
    doRefresh();
    doUpdateControls();
    descriptionValue.setText("").setFocused();
  }
}

auto StateManager::doReset() -> void {
  if(MessageDialog().setParent(*toolsManager).setText("Permanently erase all slots?").question() == "Yes") {
    for(auto slot : range(Slots)) file::remove(program->stateName(1 + slot, true));
    doRefresh();
    doUpdateControls();
  }
}

auto StateManager::doErase() -> void {
  if(auto item = stateList.selected()) {
    file::remove(program->stateName(1 + item.offset(), true));
    doRefresh();
    doUpdateControls();
  }
}
