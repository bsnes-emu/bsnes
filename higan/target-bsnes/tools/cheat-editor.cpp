CheatEditor::CheatEditor(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Edit::Replace);
  setText("Cheat Editor");

  layout.setMargin(5);
  cheatList.append(TableViewHeader().setVisible()
    .append(TableViewColumn())
    .append(TableViewColumn().setText("Slot").setForegroundColor({0, 128, 0}).setAlignment(1.0))
    .append(TableViewColumn().setText("Code(s)"))
    .append(TableViewColumn().setText("Description").setExpandable())
  );
  for(uint slot : range(Slots)) {
    cheatList.append(TableViewItem()
      .append(TableViewCell().setCheckable())
      .append(TableViewCell().setText(1 + slot))
      .append(TableViewCell())
      .append(TableViewCell())
    );
  }
  cheatList.onChange([&] { doChangeSelected(); });
  cheatList.onToggle([&](auto cell) {
    cheats[cell.parent().offset()].enabled = cell.checked();
    this->synchronizeCodes();
  });
  codeLabel.setText("Code(s):");
  codeValue.setEnabled(false).onChange([&] { doModify(); });
  descriptionLabel.setText("Description:");
  descriptionValue.setEnabled(false).onChange([&] { doModify(); });
  findCodesButton.setText("Find Codes ...").onActivate([&] { cheatDatabase->findCheats(); });
  resetButton.setText("Reset").onActivate([&] { doReset(); });
  eraseButton.setText("Erase").onActivate([&] { doErase(); });

  //do not display "Find Codes" button if there is no cheat database to look up codes in
  if(!file::exists(locate("cheats.bml"))) findCodesButton.setVisible(false);
}

auto CheatEditor::doChangeSelected() -> void {
  if(auto item = cheatList.selected()) {
    auto& cheat = cheats[item.offset()];
    codeValue.setEnabled(true).setText(cheat.code);
    descriptionValue.setEnabled(true).setText(cheat.description);
    eraseButton.setEnabled(true);
  } else {
    codeValue.setEnabled(false).setText("");
    descriptionValue.setEnabled(false).setText("");
    eraseButton.setEnabled(false);
  }
}

auto CheatEditor::doModify() -> void {
  if(auto item = cheatList.selected()) {
    auto& cheat = cheats[item.offset()];
    cheat.code = codeValue.text();
    cheat.description = descriptionValue.text();
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::doRefresh() -> void {
  for(uint slot : range(Slots)) {
    auto& cheat = cheats[slot];
    if(cheat.code || cheat.description) {
      auto codes = cheat.code.split("+");
      if(codes.size() > 1) codes[0].append("+...");
      cheatList.item(slot).cell(0).setChecked(cheat.enabled);
      cheatList.item(slot).cell(2).setText(codes[0]);
      cheatList.item(slot).cell(3).setText(cheat.description).setForegroundColor({0, 0, 0});
    } else {
      cheatList.item(slot).cell(0).setChecked(false);
      cheatList.item(slot).cell(2).setText("");
      cheatList.item(slot).cell(3).setText("<empty>").setForegroundColor({128, 128, 128});
    }
  }
  cheatList.resizeColumns();
}

auto CheatEditor::doReset(bool force) -> void {
  if(force || MessageDialog().setParent(*toolsWindow).setText("Permamently erase all cheats?").question() == "Yes") {
    for(auto& cheat : cheats) cheat = {};
    for(auto& item : cheatList.items()) item.cell(0).setChecked(false);
    doChangeSelected();
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::doErase() -> void {
  if(auto item = cheatList.selected()) {
    cheats[item.offset()] = {};
    codeValue.setText("");
    descriptionValue.setText("");
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::synchronizeCodes() -> void {
  string_vector codes;
  for(auto& cheat : cheats) {
    if(!cheat.enabled || !cheat.code) continue;
    codes.append(cheat.code);
  }
  emulator->cheatSet(codes);
}

auto CheatEditor::addCode(bool enabled, string code, string description) -> bool {
  for(auto& cheat : cheats) {
    if(cheat.code || cheat.description) continue;
    cheat.enabled = enabled;
    cheat.code = code;
    cheat.description = description;
    return true;
  }
  return false;
}

auto CheatEditor::loadCheats() -> void {
  doReset(true);
  auto location = program->path("Cheats", program->superNintendo.location, ".cht");
  auto document = BML::unserialize(string::read(location));
  for(auto cheat : document.find("cheat")) {
    if(!addCode((bool)cheat["enabled"], cheat["code"].text(), cheat["description"].text())) break;
  }
  doRefresh();
  synchronizeCodes();
}

auto CheatEditor::saveCheats() -> void {
  string document;
  for(auto& cheat : cheats) {
    if(!cheat.code && !cheat.description) continue;
    document.append("cheat\n");
    document.append("  description: ", cheat.description, "\n");
    document.append("  code: ", cheat.code, "\n");
  if(cheat.enabled)
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
