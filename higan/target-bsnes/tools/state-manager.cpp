auto StateWindow::create() -> void {
  layout.setPadding(5);
  nameLabel.setText("Name:");
  nameValue.onActivate([&] { if(acceptButton.enabled()) acceptButton.doActivate(); });
  nameValue.onChange([&] { doChange(); });
  acceptButton.onActivate([&] { doAccept(); });
  cancelButton.setText("Cancel").onActivate([&] { setVisible(false); });

  setSize({400, layout.minimumSize().height()});
  setDismissable();
}

auto StateWindow::show(string name) -> void {
  setProperty("type", {name.split("/").first(), "/"});
  setProperty("name", {name.split("/").last()});
  nameValue.setText(property("name"));
  doChange();
  setTitle(!property("name") ? "Add State" : "Rename State");
  setCentered(*toolsWindow);
  setVisible();
  setFocused();
  nameValue.setFocused();
  acceptButton.setText(!property("name") ? "Add" : "Rename");
}

auto StateWindow::doChange() -> void {
  auto name = nameValue.text().strip();
  bool valid = name && !name.contains("\\\"\t/:*?<>|");
  if(property("name") && name != property("name")) {
    //don't allow a state to be renamed to the same name as an existing state (as this would overwrite it)
    if(program.hasState({property("type"), name})) valid = false;
  }
  nameValue.setBackgroundColor(valid ? Color{} : Color{255, 224, 224});
  acceptButton.setEnabled(valid);
}

auto StateWindow::doAccept() -> void {
  string name = {property("type"), nameValue.text().strip()};
  if(acceptButton.text() == "Add") {
    stateManager.createState(name);
  } else {
    stateManager.modifyState(name);
  }
  setVisible(false);
}

auto StateManager::create() -> void {
  setIcon(Icon::Application::FileManager);
  setText("State Manager");

  layout.setPadding(5);
  stateLayout.setAlignment(0.0);
  stateList.setBatchable();
  stateList.setHeadered();
  stateList.setSortable();
  stateList.onActivate([&] { loadButton.doActivate(); });
  stateList.onChange([&] { updateSelection(); });
  stateList.onSort([&](TableViewColumn column) {
    column.setSorting(column.sorting() == Sort::Ascending ? Sort::Descending : Sort::Ascending);
    stateList.sort();
  });
  stateList.onSize([&] {
    stateList.resizeColumns();
  });
  categoryLabel.setText("Category:");
  categoryOption.append(ComboButtonItem().setText("Managed States").setProperty("type", "Managed/"));
  categoryOption.append(ComboButtonItem().setText("Quick States").setProperty("type", "Quick/"));
  categoryOption.onChange([&] { loadStates(); });
  statePreviewSeparator.setColor({192, 192, 192});
  statePreviewLabel.setFont(Font().setBold()).setText("Preview");
  loadButton.setText("Load").onActivate([&] {
    if(auto item = stateList.selected()) program.loadState(item.property("name"));
  });
  saveButton.setText("Save").onActivate([&] {
    if(auto item = stateList.selected()) program.saveState(item.property("name"));
  });
  addButton.setText("Add").onActivate([&] {
    stateWindow.show(type());
  });
  editButton.setText("Rename").onActivate([&] {
    if(auto item = stateList.selected()) stateWindow.show(item.property("name"));
  });
  removeButton.setText("Remove").onActivate([&] {
    removeStates();
  });
}

auto StateManager::type() const -> string {
  return categoryOption.selected().property("type");
}

auto StateManager::loadStates() -> void {
  stateList.reset();
  stateList.append(TableViewColumn().setText("Name").setSorting(Sort::Ascending).setExpandable());
  stateList.append(TableViewColumn().setText("Date").setForegroundColor({160, 160, 160}));
  auto type = this->type();
  for(auto& state : program.availableStates(type)) {
    TableViewItem item{&stateList};
    item.setProperty("name", state.name);
    item.append(TableViewCell().setText(state.name.trimLeft(type, 1L)));
    item.append(TableViewCell().setText(chrono::local::datetime(state.date)));
  }
  stateList.resizeColumns().doChange();
}

auto StateManager::createState(string name) -> void {
  for(auto& item : stateList.items()) {
    item.setSelected(false);
  }
  program.saveState(name);
  for(auto& item : stateList.items()) {
    item.setSelected(item.property("name") == name);
  }
  stateList.doChange();
}

auto StateManager::modifyState(string name) -> void {
  if(auto item = stateList.selected()) {
    string from = item.property("name");
    string to = name;
    if(from != to) {
      program.renameState(from, to);
      for(auto& item : stateList.items()) {
        item.setSelected(item.property("name") == to);
      }
      stateList.doChange();
    }
  }
}

auto StateManager::removeStates() -> void {
  if(auto batched = stateList.batched()) {
    if(MessageDialog("Are you sure you want to permanently remove the selected state(s)?")
    .setParent(*toolsWindow).question() == "Yes") {
      auto lock = acquire();
      for(auto& item : batched) program.removeState(item.property("name"));
      loadStates();
    }
  }
}

auto StateManager::updateSelection() -> void {
  auto batched = stateList.batched();
  statePreview.setVisible(batched.size() == 1);
  loadButton.setEnabled(batched.size() == 1);
  saveButton.setEnabled(batched.size() == 1);
  editButton.setEnabled(batched.size() == 1);
  addButton.setVisible(type() != "Quick/");
  editButton.setVisible(type() != "Quick/");
  removeButton.setEnabled(batched.size() >= 1);

  statePreview.setColor({0, 0, 0});
  if(batched.size() == 1) {
    if(auto saveState = program.loadStateData(batched.first().property("name"))) {
      if(saveState.size() >= 3 * sizeof(uint)) {
        uint signature  = memory::readl<sizeof(uint)>(saveState.data() + 0 * sizeof(uint));
        uint serializer = memory::readl<sizeof(uint)>(saveState.data() + 1 * sizeof(uint));
        uint preview    = memory::readl<sizeof(uint)>(saveState.data() + 2 * sizeof(uint));
        if(signature == Program::State::Signature && preview) {
          uint offset = 3 * sizeof(uint) + serializer;
          auto preview = Decode::RLE<uint16_t>(saveState.data() + offset, max(offset, saveState.size()) - offset);
          image icon{0, 15, 0x8000, 0x7c00, 0x03e0, 0x001f};
          icon.copy(preview.data(), 256 * sizeof(uint16_t), 256, 240);
          icon.transform();
          statePreview.setIcon(icon);
        }
      }
    }
  }
}

auto StateManager::stateEvent(string name) -> void {
  if(locked() || !name.beginsWith(type())) return;
  auto selected = stateList.selected().property("name");
  loadStates();
  for(auto& item : stateList.items()) {
    item.setSelected(item.property("name") == selected);
  }
  stateList.doChange();
}
