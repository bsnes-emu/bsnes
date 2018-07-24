CheatEditor::CheatEditor(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Edit::Replace);
  setText("Cheat Editor");

  layout.setPadding(5);
  cheatList.append(TableViewHeader().setVisible()
    .append(TableViewColumn())
    .append(TableViewColumn().setText("Slot").setForegroundColor({0, 128, 0}).setAlignment(1.0))
    .append(TableViewColumn().setText("Code(s)"))
    .append(TableViewColumn().setText("Description").setExpandable())
  );
  for(auto slot : range(Slots)) {
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
  codeValue.onChange([&] { doModify(); });
  descriptionLabel.setText("Description:");
  descriptionValue.onChange([&] { doModify(); });
  findCodesButton.setText("Find Codes ...").onActivate([&] { cheatDatabase->findCodes(); });
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
  for(auto slot : range(Slots)) {
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
  if(force || MessageDialog().setParent(*toolsManager).setText("Permanently erase all cheats?").question() == "Yes") {
    for(auto& cheat : cheats) cheat = {};
    for(auto& item : cheatList.items()) item.cell(0).setChecked(false);
    doChangeSelected();
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::doErase() -> void {
  if(auto item = cheatList.selected()) {
    auto& cheat = cheats[item.offset()];
    cheats[item.offset()] = {};
    codeValue.setText("");
    descriptionValue.setText("");
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::synchronizeCodes() -> void {
  if(!emulator) return;

  string_vector codes;
  for(auto& cheat : cheats) {
    if(!cheat.enabled || !cheat.code) continue;
    codes.append(cheat.code);
  }

  emulator->cheatSet(codes);
}

//returns true if code was added
//returns false if there are no more free slots for additional codes
auto CheatEditor::addCode(const string& code, const string& description, bool enabled) -> bool {
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
  auto contents = string::read({program->gamePaths(1), "higan/cheats.bml"});
  auto document = BML::unserialize(contents);
  for(auto cheat : document["cartridge"].find("cheat")) {
    if(!addCode(cheat["code"].text(), cheat["description"].text(), (bool)cheat["enabled"])) break;
  }
  doRefresh();
  synchronizeCodes();
}

auto CheatEditor::saveCheats() -> void {
  if(!emulator) return;
  string document = {"cartridge sha256:", emulator->sha256(), "\n"};
  uint count = 0;
  for(auto& cheat : cheats) {
    if(!cheat.code && !cheat.description) continue;
    document.append("  cheat", cheat.enabled ? " enabled" : "", "\n");
    document.append("    description:", cheat.description, "\n");
    document.append("    code:", cheat.code, "\n");
    count++;
  }
  if(count) {
    directory::create({program->gamePaths(1), "higan/"});
    file::write({program->gamePaths(1), "higan/cheats.bml"}, document);
  } else {
    file::remove({program->gamePaths(1), "higan/cheats.bml"});
  }
  doReset(true);
}
