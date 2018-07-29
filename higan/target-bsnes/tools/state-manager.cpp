StateWindow::StateWindow() {
  stateWindow = this;

  layout.setPadding(5);
  nameLabel.setText("Name:");
  nameValue.onActivate([&] {
    if(acceptButton.enabled()) acceptButton.doActivate();
  });
  nameValue.onChange([&] {
    doChange();
  });
  acceptButton.onActivate([&] {
    doAccept();
  });
  cancelButton.setText("Cancel").onActivate([&] {
    setVisible(false);
  });

  setSize({400, layout.minimumSize().height()});
  setDismissable();
}

auto StateWindow::show(string name) -> void {
  nameValue.setText(name).setProperty("input", name);
  doChange();
  setTitle(!name ? "Add State" : "Edit State");
  setCentered(*toolsWindow);
  setVisible();
  setFocused();
  nameValue.setFocused();
  acceptButton.setText(!name ? "Add" : "Edit");
}

auto StateWindow::doChange() -> void {
  bool valid = true;
  auto name = nameValue.text().strip();
  if(!name) valid = false;
  for(auto c : name) {
    if(c == '\\'
    || c == '\"'
    || c == '\t'
    || c == '/'
    || c == ':'
    || c == '*'
    || c == '?'
    || c == '<'
    || c == '>'
    || c == '|') valid = false;
  }
  if(auto input = nameValue.property("input")) {
    if(name != input && file::exists({program->statePath(), "managed/", name, ".bst"})) valid = false;
  }
  nameValue.setBackgroundColor(valid ? Color{} : Color{255, 224, 224});
  acceptButton.setEnabled(valid);
}

auto StateWindow::doAccept() -> void {
  if(acceptButton.text() == "Add") {
    toolsWindow->stateManager.createState(nameValue.text());
  } else {
    toolsWindow->stateManager.modifyState(nameValue.text());
  }
  setVisible(false);
}

StateManager::StateManager(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Application::FileManager);
  setText("State Manager");

  layout.setPadding(5);
  stateList.setBatchable();
  stateList.onActivate([&] {
    editButton.doActivate();
  });
  stateList.onChange([&] {
    auto batched = stateList.batched();
    loadButton.setEnabled(batched.size() == 1);
    saveButton.setEnabled(batched.size() == 1);
    editButton.setEnabled(batched.size() == 1);
    removeButton.setEnabled(batched.size() >= 1);
  });
  loadButton.setText("Load").onActivate([&] {
    if(auto item = stateList.selected()) {
      string filename = {"managed/", item.cell(0).text()};
      program->loadState(filename);
    }
  });
  saveButton.setText("Save").onActivate([&] {
    if(auto item = stateList.selected()) {
      string filename = {"managed/", item.cell(0).text()};
      program->saveState(filename);
      item.cell(1).setText(chrono::local::datetime(program->stateTimestamp(filename)));
    }
  });
  addButton.setText("Add").onActivate([&] {
    stateWindow->show();
  });
  editButton.setText("Edit").onActivate([&] {
    if(auto item = stateList.selected()) {
      stateWindow->show(item.cell(0).text());
    }
  });
  removeButton.setText("Remove").onActivate([&] {
    removeStates();
  });
}

auto StateManager::loadStates() -> void {
  stateList.reset();
  stateList.append(TableViewHeader()
    .append(TableViewColumn().setText("Name").setExpandable())
    .append(TableViewColumn().setText("Date").setForegroundColor({160, 160, 160}))
  );
  for(auto& filename : program->availableStates("managed/")) {
    stateList.append(TableViewItem()
      .append(TableViewCell().setText(string{filename}.trimLeft("managed/", 1L)))
      .append(TableViewCell().setText(chrono::local::datetime(program->stateTimestamp(filename))))
    );
  }
  stateList.resizeColumns().doChange();
}

auto StateManager::createState(string name) -> void {
  program->saveState({"managed/", name});
  loadStates();
  for(auto& item : stateList.items()) {
    if(item.cell(0).text() == name) item.setSelected();
  }
  stateList.doChange();
}

auto StateManager::modifyState(string name) -> void {
  if(auto item = stateList.selected()) {
    string from = {"managed/", item.cell(0).text()};
    string to = {"managed/", name};
    if(from != to) {
      program->renameState(from, to);
      loadStates();
      for(auto& item : stateList.items()) {
        if(item.cell(0).text() == name) item.setSelected();
      }
      stateList.doChange();
    }
  }
}

auto StateManager::removeStates() -> void {
  if(auto batched = stateList.batched()) {
    if(MessageDialog("Are you sure you want to permanently remove the selected state(s)?")
    .setParent(*toolsWindow).question() == "Yes") {
      for(auto& item : batched) {
        program->removeState({"managed/", item.cell(0).text()});
      }
      loadStates();
    }
  }
}
