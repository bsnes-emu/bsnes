CheatDatabase::CheatDatabase() {
  cheatDatabase = this;

  layout.setMargin(5);
  selectAllButton.setText("Select All").onActivate([&] {
    for(auto item : cheatList.items()) item.setChecked(true);
  });
  unselectAllButton.setText("Unselect All").onActivate([&] {
    for(auto item : cheatList.items()) item.setChecked(false);
  });
  addCheatsButton.setText("Add Cheats").onActivate([&] {
    addCheats();
  });

  setSize({800, 400});
  setAlignment({0.5, 1.0});
  setDismissable();
}

auto CheatDatabase::findCheats() -> void {
  auto sha256 = emulator->sha256();

  auto document = BML::unserialize(string::read(locate("cheats.bml")));
  for(auto game : document.find("cartridge")) {
    if(game["sha256"].text() != sha256) continue;

    cheatList.reset();
    for(auto cheat : game.find("cheat")) {
      cheatList.append(ListViewItem()
        .setCheckable()
        .setText(cheat["description"].text())
        .setProperty("code", cheat["code"].text())
      );
    }

    setTitle(game["name"].text());
    setVisible();
    return;
  }

  MessageDialog().setParent(*toolsWindow).setText("Sorry, no cheats were found for this game.").information();
}

auto CheatDatabase::addCheats() -> void {
  for(auto item : cheatList.items()) {
    if(item.checked()) {
      toolsWindow->cheatEditor.addCheat(item.text(), item.property("code"));
    }
  }
  setVisible(false);
  toolsWindow->cheatEditor.synchronizeCodes();
}

//

CheatWindow::CheatWindow() {
  cheatWindow = this;

  layout.setMargin(5);
  nameLabel.setText("Name:");
  nameValue.onChange([&] { doChange(); });
  codeLabel.setText("Code:");
  codeValue.onChange([&] { doChange(); });
  enabledOption.setText("Enabled");
  acceptButton.onActivate([&] { doAccept(); });
  cancelButton.setText("Cancel");

  setSize({480, layout.minimumSize().height()});
  setDismissable();
}

auto CheatWindow::show(TableViewItem item) -> void {
  this->item = item;
  nameValue.setText(item.cell(1).text());
  codeValue.setText(item.property("code"));
  enabledOption.setChecked(item.cell(0).checked());
  doChange();
  setTitle(!item ? "Add Cheat Code" : "Edit Cheat Code");
  setCentered(*toolsWindow);
  setVisible();
  setFocused();
  nameValue.setFocused();
  acceptButton.setText(!item ? "Add" : "Edit");
}

auto CheatWindow::doChange() -> void {
  bool valid = true;
  nameValue.setBackgroundColor(nameValue.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  codeValue.setBackgroundColor(codeValue.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  acceptButton.setEnabled(valid);
}

auto CheatWindow::doAccept() -> void {
  if(item) {
    item.cell(0).setChecked(enabledOption.checked());
    item.cell(1).setText(nameValue.text());
    item.setProperty("code", codeValue.text());
  } else {
    toolsWindow->cheatEditor.addCheat(nameValue.text(), codeValue.text(), enabledOption.checked());
  }
  setVisible(false);
}

//

CheatEditor::CheatEditor(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Edit::Replace);
  setText("Cheat Editor");

  layout.setMargin(5);
  cheatList.onActivate([&] { modifyButton.doActivate(); });
  cheatList.onChange([&] {
    auto selected = cheatList.selected();
    upButton.setEnabled((bool)selected && selected.offset() != 0);
    downButton.setEnabled((bool)selected && selected.offset() != cheatList.itemCount() - 1);
    modifyButton.setEnabled((bool)selected);
    removeButton.setEnabled((bool)selected);
  });
  cheatList.onToggle([&](TableViewCell) { synchronizeCodes(); });
  upButton.setIcon(Icon::Go::Up).onActivate([&] {
    if(auto item = cheatList.selected()) {
      swap(item.offset(), item.offset() - 1);
    }
  });
  downButton.setIcon(Icon::Go::Down).onActivate([&] {
    if(auto item = cheatList.selected()) {
      swap(item.offset(), item.offset() + 1);
    }
  });
  findCheatsButton.setText("Find Cheats ...").onActivate([&] {
    cheatDatabase->findCheats();
  });
  resetButton.setText("Reset").onActivate([&] {
    if(MessageDialog("Are you sure you want to permanently erase all cheat codes?").setParent(*toolsWindow).question() == "Yes") {
      cheatList.reset().append(TableViewHeader()
        .append(TableViewColumn())
        .append(TableViewColumn().setExpandable())
        .setVisible(false)
      );
      synchronizeCodes();
    }
  });
  appendButton.setText("Add").onActivate([&] {
    cheatWindow->show();
  });
  modifyButton.setText("Edit").onActivate([&] {
    if(auto item = cheatList.selected()) {
      cheatWindow->show(item);
    }
  });
  removeButton.setText("Remove").onActivate([&] {
    if(auto item = cheatList.selected()) {
      cheatList.remove(item).doChange();
    }
  });

  //do not display "Find Cheats" button if there is no cheat database available
  if(!file::exists(locate("cheats.bml"))) findCheatsButton.setVisible(false);
}

auto CheatEditor::swap(uint x, uint y) -> void {
  auto itemX = cheatList.item(x);
  auto itemY = cheatList.item(y);
  auto enabled = itemX.cell(0).checked();
  auto name = itemX.cell(1).text();
  auto code = itemX.property("code");
  itemX.cell(0).setChecked(itemY.cell(0).checked());
  itemX.cell(1).setText(itemY.cell(1).text());
  itemX.setProperty("code", itemY.property("code"));
  itemY.cell(0).setChecked(enabled);
  itemY.cell(1).setText(name);
  itemY.setProperty("code", code);
  itemY.setSelected();
  cheatList.doChange();
}

auto CheatEditor::synchronizeCodes() -> void {
  string_vector codes;
  for(auto item : cheatList.items()) {
    if(item.cell(0).checked()) codes.append(item.property("code"));
  }
  emulator->cheatSet(codes);
}

auto CheatEditor::addCheat(string name, string code, bool enabled) -> void {
  cheatList.append(TableViewItem()
    .append(TableViewCell().setChecked(enabled))
    .append(TableViewCell().setText(name))
    .setProperty("code", code)
  ).resizeColumns();
}

auto CheatEditor::loadCheats() -> void {
  cheatList.reset().append(TableViewHeader()
    .append(TableViewColumn())
    .append(TableViewColumn().setExpandable())
    .setVisible(false)
  );
  auto location = program->path("Cheats", program->superNintendo.location, ".cht");
  auto document = BML::unserialize(string::read(location));
  for(auto cheat : document.find("cheat")) {
    addCheat(cheat["name"].text(), cheat["code"].text(), (bool)cheat["enabled"]);
  }
  cheatList.doChange();
  synchronizeCodes();
}

auto CheatEditor::saveCheats() -> void {
  string document;
  for(auto item : cheatList.items()) {
    document.append("cheat\n");
    document.append("  name: ", item.cell(1).text(), "\n");
    document.append("  code: ", item.property("code"), "\n");
  if(item.cell(0).checked())
    document.append("  enabled\n");
    document.append("\n");
  }
  auto location = program->path("Cheats", program->superNintendo.location, ".cht");
  if(document) {
    file::write(location, document);
  } else {
    file::remove(location);
  }
}
