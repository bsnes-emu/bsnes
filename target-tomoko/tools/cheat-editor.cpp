CheatEditor::CheatEditor(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Edit::Replace);
  setText("Cheat Editor");

  layout.setMargin(5);
  cheatList.append(ListViewColumn().setText("Slot").setForegroundColor({0, 128, 0}).setHorizontalAlignment(1.0));
  cheatList.append(ListViewColumn().setText("Code(s)").setWidth(0));
  cheatList.append(ListViewColumn().setText("Description").setWidth(~0));
  for(auto slot : range(Slots)) cheatList.append(ListViewItem().setText(0, 1 + slot));
  cheatList.setCheckable();
  cheatList.setHeaderVisible();
  cheatList.onChange([&] { doChange(); });
  cheatList.onToggle([&](sListViewItem) { synchronizeCodes(); });
  codeLabel.setText("Code(s):");
  codeValue.onChange([&] { doModify(); });
  descriptionLabel.setText("Description:");
  descriptionValue.onChange([&] { doModify(); });
  findCodesButton.setText("Find Codes ...").onActivate([&] { cheatDatabase->findCodes(); });
  resetButton.setText("Reset").onActivate([&] { doReset(); });
  eraseButton.setText("Erase").onActivate([&] { doErase(); });
}

auto CheatEditor::doChange() -> void {
  if(auto item = cheatList.selected()) {
    unsigned slot = item->offset();
    codeValue.setEnabled(true).setText(cheats[slot].code);
    descriptionValue.setEnabled(true).setText(cheats[slot].description);
    eraseButton.setEnabled(true);
  } else {
    codeValue.setEnabled(false).setText("");
    descriptionValue.setEnabled(false).setText("");
    eraseButton.setEnabled(false);
  }
}

auto CheatEditor::doModify() -> void {
  if(auto item = cheatList.selected()) {
    unsigned slot = item->offset();
    cheats[slot].code = codeValue.text();
    cheats[slot].description = descriptionValue.text();
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::doRefresh() -> void {
  for(auto slot : range(Slots)) {
    if(cheats[slot].code || cheats[slot].description) {
      lstring codes = cheats[slot].code.split("+");
      if(codes.size() > 1) codes[0].append("+...");
      cheatList.item(slot)->setText(1, codes[0]).setText(2, cheats[slot].description);
    } else {
      cheatList.item(slot)->setText(1, "").setText(2, "(empty)");
    }
  }

  cheatList.resizeColumns();
}

auto CheatEditor::doReset() -> void {
  if(MessageDialog().setParent(*toolsManager).setText("Permanently erase all slots?").question() == 0) {
    for(auto& cheat : cheats) {
      cheat.code = "";
      cheat.description = "";
    }
    cheatList.setSelected(false);
    doChange();
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::doErase() -> void {
  if(auto item = cheatList.selected()) {
    unsigned slot = item->offset();
    cheats[slot].code = "";
    cheats[slot].description = "";
    codeValue.setText("");
    descriptionValue.setText("");
    doRefresh();
    synchronizeCodes();
  }
}

auto CheatEditor::synchronizeCodes() -> void {
  if(!emulator) return;

  lstring codes;
  for(auto slot : range(Slots)) {
    if(!cheatList.item(slot)->checked()) continue;
    if(!cheats[slot].code) continue;
    codes.append(cheats[slot].code);
  }

  emulator->cheatSet(codes);
}

//returns true if code was added
//returns false if there are no more free slots for additional codes
auto CheatEditor::addCode(const string& code, const string& description) -> bool {
  for(auto& cheat : cheats) {
    if(cheat.code || cheat.description) continue;

    cheat.code = code;
    cheat.description = description;
    doRefresh();
    return true;
  }

  return false;
}
