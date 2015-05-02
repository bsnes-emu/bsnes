CheatEditor::CheatEditor(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Edit::Replace);
  setText("Cheat Editor");

  layout.setMargin(5);
  cheatList.append(ListViewColumn().setText("Slot").setForegroundColor({0, 128, 0}).setHorizontalAlignment(1.0));
  cheatList.append(ListViewColumn().setText("Code(s)"));
  cheatList.append(ListViewColumn().setText("Description").setWidth(~0));
  for(auto slot : range(Slots)) cheatList.append(ListViewItem().setText(0, 1 + slot));
  cheatList.setCheckable();
  cheatList.setHeaderVisible();
  cheatList.onChange([&] { doChangeSelected(); });
  cheatList.onToggle([&](sListViewItem item) {
    cheats[item->offset()].enabled = item->checked();
    synchronizeCodes();
  });
  codeLabel.setText("Code(s):");
  codeValue.onChange([&] { doModify(); });
  descriptionLabel.setText("Description:");
  descriptionValue.onChange([&] { doModify(); });
  findCodesButton.setText("Find Codes ...").onActivate([&] { cheatDatabase->findCodes(); });
  resetButton.setText("Reset").onActivate([&] { doReset(); });
  eraseButton.setText("Erase").onActivate([&] { doErase(); });
}

auto CheatEditor::doChangeSelected() -> void {
  if(auto item = cheatList.selected()) {
    auto& cheat = cheats[item->offset()];
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
    auto& cheat = cheats[item->offset()];
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
      lstring codes = cheat.code.split("+");
      if(codes.size() > 1) codes[0].append("+...");
      cheatList.item(slot)->setChecked(cheat.enabled).setText(1, codes[0]).setText(2, cheat.description);
    } else {
      cheatList.item(slot)->setChecked(false).setText(1, "").setText(2, "(empty)");
    }
  }

  cheatList.resizeColumns();
}

auto CheatEditor::doReset(bool force) -> void {
  if(force || MessageDialog().setParent(*toolsManager).setText("Permanently erase all slots?").question() == 0) {
    for(auto& cheat : cheats) {
      cheat.enabled = false;
      cheat.code = "";
      cheat.description = "";
    }
    cheatList.setSelected(false);
    doChangeSelected();
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::doErase() -> void {
  if(auto item = cheatList.selected()) {
    auto& cheat = cheats[item->offset()];
    cheat.enabled = false;
    cheat.code = "";
    cheat.description = "";
    codeValue.setText("");
    descriptionValue.setText("");
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::synchronizeCodes() -> void {
  if(!emulator) return;

  lstring codes;
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
  auto contents = string::read({program->folderPaths[0], "cheats.bml"});
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
  unsigned count = 0;
  for(auto& cheat : cheats) {
    if(!cheat.code && !cheat.description) continue;
    document.append("  cheat", cheat.enabled ? " enabled" : "", "\n");
    document.append("    description:", cheat.description, "\n");
    document.append("    code:", cheat.code, "\n");
    count++;
  }
  if(count) {
    file::write({program->folderPaths[0], "cheats.bml"}, document);
  } else {
    file::remove({program->folderPaths[0], "cheats.bml"});
  }
  doReset(true);
}
