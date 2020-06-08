auto CheatDatabase::create() -> void {
  layout.setPadding(5_sx);
  selectAllButton.setText("Select All").onActivate([&] {
    for(auto item : cheatList.items()) item.setChecked(true);
  });
  unselectAllButton.setText("Unselect All").onActivate([&] {
    for(auto item : cheatList.items()) item.setChecked(false);
  });
  addCheatsButton.setText("Add Cheats").onActivate([&] {
    addCheats();
  });

  setSize({800_sx, 400_sx});
  setAlignment({0.5, 1.0});
  setDismissable();
}

auto CheatDatabase::findCheats() -> void {
  //hack to locate Super Game Boy cheat codes
  auto sha256a = emulator->hashes()(0, "none");
  auto sha256b = emulator->hashes()(1, "none");

  auto document = BML::unserialize(string::read(locate("Database/Cheat Codes.bml")));
  for(auto game : document.find("cartridge")) {
    if(game["sha256"].text() != sha256a
    && game["sha256"].text() != sha256b) continue;

    cheatList.reset();
    for(auto cheat : game.find("cheat")) {
      //convert old cheat format (address/data and address/compare/data)
      //to new cheat format (address=data and address=compare?data)
      auto codes = cheat["code"].text().split("+").strip();
      for(auto& code : codes) {
        code.replace("/", "=", 1L);
        code.replace("/", "?", 1L);
      }
      auto code = codes.merge("+");
      cheatList.append(ListViewItem()
        .setCheckable()
        .setText(cheat["description"].text())
        .setAttribute("code", code)
      );
    }

    setTitle(game["name"].text());
    setVisible();
    return;
  }

  MessageDialog().setAlignment(*toolsWindow).setText("Sorry, no cheats were found for this game.").information();
}

auto CheatDatabase::addCheats() -> void {
  for(auto item : cheatList.items()) {
    if(item.checked()) {
      cheatEditor.addCheat({item.text(), item.attribute("code"), false});
    }
  }
  setVisible(false);
}

//

auto CheatWindow::create() -> void {
  layout.setPadding(5_sx);
  tableLayout.setSize({2, 2});
  tableLayout.cell(0).setAlignment({1.0, 0.5});
  tableLayout.cell(2).setAlignment({1.0, 0.0});
  nameLabel.setText("Name:");
  nameValue.onActivate([&] { if(acceptButton.enabled()) acceptButton.doActivate(); });
  nameValue.onChange([&] { doChange(); });
  codeLabel.setText("Code(s):");
  codeValue.setFont(Font().setFamily(Font::Mono));
  codeValue.onChange([&] { doChange(); });
  enableOption.setText("Enable");
  acceptButton.onActivate([&] { doAccept(); });
  cancelButton.setText("Cancel").onActivate([&] { setVisible(false); });

  setSize({400_sx, layout.minimumSize().height() + 100_sx});
  setDismissable();
}

auto CheatWindow::show(Cheat cheat) -> void {
  nameValue.setText(cheat.name);
  codeValue.setText(cheat.code.split("+").strip().merge("\n"));
  enableOption.setChecked(cheat.enable);
  doChange();
  setTitle(!cheat.name ? "Add Cheat" : "Edit Cheat");
  setAlignment(*toolsWindow);
  setVisible();
  setFocused();
  nameValue.setFocused();
  acceptButton.setText(!cheat.name ? "Add" : "Edit");
}

auto CheatWindow::doChange() -> void {
  bool valid = true;
  nameValue.setBackgroundColor(nameValue.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  codeValue.setBackgroundColor(codeValue.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  acceptButton.setEnabled(valid);
}

auto CheatWindow::doAccept() -> void {
  auto codes = codeValue.text().downcase().transform("+", "\n").split("\n").strip();
  string invalid;  //if empty after below for-loop, code is considered valid
  for(auto& code : codes) {
    if(!program.gameBoy.program) {
      if(!cheatEditor.decodeSNES(code)) {
        invalid =
          "Invalid code(s), please only use codes in the following format:\n"
          "\n"
          "Game Genie (eeee-eeee)\n"
          "Pro Action Replay (aaaaaadd)\n"
          "higan (aaaaaa=dd)\n"
          "higan (aaaaaa=cc?dd)";
      }
    } else {
      if(!cheatEditor.decodeGB(code)) {
        invalid =
          "Invalid code(s), please only use codes in the following format:\n"
          "\n"
          "Game Genie (eee-eee)\n"
          "Game Genie (eee-eee-eee)\n"
          "GameShark (01ddaaaa)\n"
          "higan (aaaa=dd)\n"
          "higan (aaaa=cc?dd)";
      }
    }
  }
  if(invalid) return (void)MessageDialog().setAlignment(*toolsWindow).setText(invalid).error();

  Cheat cheat = {nameValue.text().strip(), codes.merge("+"), enableOption.checked()};
  if(acceptButton.text() == "Add") {
    cheatEditor.addCheat(cheat);
  } else {
    cheatEditor.editCheat(cheat);
  }
  setVisible(false);
}

//

auto CheatEditor::create() -> void {
  setCollapsible();
  setVisible(false);

  cheatList.setBatchable();
  cheatList.setHeadered();
  cheatList.setSortable();
  cheatList.onActivate([&](auto cell) {
    //kind of a hack: toggling a cheat code twice quickly (onToggle) will call onActivate.
    //do not trigger the CheatWindow unless it's been at least two seconds since a cheat code was last toggled on or off.
    if(chrono::timestamp() - activateTimeout < 2) return;
    editButton.doActivate();
  });
  cheatList.onChange([&] {
    auto batched = cheatList.batched();
    editButton.setEnabled(batched.size() == 1);
    removeButton.setEnabled(batched.size() >= 1);
  });
  cheatList.onToggle([&](TableViewCell cell) {
    activateTimeout = chrono::timestamp();
    if(auto item = cell->parentTableViewItem()) {
      cheats[item->offset()].enable = cell.checked();
      synchronizeCodes();
    }
  });
  cheatList.onSort([&](TableViewColumn column) {
    column.setSorting(column.sorting() == Sort::Ascending ? Sort::Descending : Sort::Ascending);
  //cheatList.sort();
  });
  cheatList.onSize([&] {
    cheatList.resizeColumns();
  });
  findCheatsButton.setText("Find Cheats ...").onActivate([&] {
    cheatDatabase.findCheats();
  });
  enableCheats.setText("Enable Cheats").setToolTip(
    "Master enable for all cheat codes.\n"
    "When unchecked, no cheat codes will be active.\n\n"
    "Use this to bypass game areas that have problems with cheats."
  ).setChecked(settings.emulator.cheats.enable).onToggle([&] {
    settings.emulator.cheats.enable = enableCheats.checked();
    if(!enableCheats.checked()) {
      program.showMessage("All cheat codes disabled");
    } else {
      program.showMessage("Active cheat codes enabled");
    }
    synchronizeCodes();
  });
  addButton.setText("Add").onActivate([&] {
    cheatWindow.show();
  });
  editButton.setText("Edit").onActivate([&] {
    if(auto item = cheatList.selected()) {
      cheatWindow.show(cheats[item.offset()]);
    }
  });
  removeButton.setText("Remove").onActivate([&] {
    removeCheats();
  });

  //hide the "Find Cheats" button if the cheat code database isn't found
  if(!file::exists(locate("Database/Cheat Codes.bml"))) findCheatsButton.setVisible(false);
}

auto CheatEditor::refresh() -> void {
  cheatList.reset();
  cheatList.append(TableViewColumn());
  cheatList.append(TableViewColumn().setText("Name").setSorting(Sort::Ascending).setExpandable());
  for(auto& cheat : cheats) {
    TableViewItem item{&cheatList};
    item.append(TableViewCell().setCheckable().setChecked(cheat.enable));
    item.append(TableViewCell().setText(cheat.name));
  }
  cheatList.resizeColumns().doChange();
}

auto CheatEditor::addCheat(Cheat cheat) -> void {
  cheats.append(cheat);
  cheats.sort();
  refresh();
  for(uint index : range(cheats.size())) {
    if(cheats[index] == cheat) { cheatList.item(index).setSelected(); break; }
  }
  cheatList.doChange();
  synchronizeCodes();
}

auto CheatEditor::editCheat(Cheat cheat) -> void {
  if(auto item = cheatList.selected()) {
    cheats[item.offset()] = cheat;
    cheats.sort();
    refresh();
    for(uint index : range(cheats.size())) {
      if(cheats[index] == cheat) { cheatList.item(index).setSelected(); break; }
    }
    cheatList.doChange();
    synchronizeCodes();
  }
}

auto CheatEditor::removeCheats() -> void {
  if(auto batched = cheatList.batched()) {
    if(MessageDialog("Are you sure you want to permanently remove the selected cheat(s)?")
    .setAlignment(*toolsWindow).question() == "Yes") {
      for(auto& item : reverse(batched)) cheats.remove(item.offset());
      cheats.sort();
      refresh();
      synchronizeCodes();
    }
  }
}

auto CheatEditor::loadCheats() -> void {
  cheats.reset();
  auto location = program.cheatPath();
  auto document = BML::unserialize(string::read(location));
  for(auto cheat : document.find("cheat")) {
    cheats.append({cheat["name"].text(), cheat["code"].text(), (bool)cheat["enable"]});
  }
  cheats.sort();
  refresh();
  synchronizeCodes();
}

auto CheatEditor::saveCheats() -> void {
  string document;
  for(auto cheat : cheats) {
    document.append("cheat\n");
    document.append("  name: ", cheat.name, "\n");
    document.append("  code: ", cheat.code, "\n");
  if(cheat.enable)
    document.append("  enable\n");
    document.append("\n");
  }
  auto location = program.cheatPath();
  if(document) {
    file::write(location, document);
  } else {
    file::remove(location);
  }
}

auto CheatEditor::synchronizeCodes() -> void {
  vector<string> codes;
  if(enableCheats.checked()) {
    for(auto& cheat : cheats) {
      if(cheat.enable) codes.append(cheat.code);
    }
  }
  emulator->cheats(codes);
}

//

auto CheatEditor::decodeSNES(string& code) -> bool {
  //Game Genie
  if(code.size() == 9 && code[4u] == '-') {
    //strip '-'
    code = {code.slice(0, 4), code.slice(5, 4)};
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //decode
    code.transform("df4709156bc8a23e", "0123456789abcdef");
    uint32_t r = toHex(code);
    //abcd efgh ijkl mnop qrst uvwx
    //ijkl qrst opab cduv wxef ghmn
    uint address =
      (!!(r & 0x002000) << 23) | (!!(r & 0x001000) << 22)
    | (!!(r & 0x000800) << 21) | (!!(r & 0x000400) << 20)
    | (!!(r & 0x000020) << 19) | (!!(r & 0x000010) << 18)
    | (!!(r & 0x000008) << 17) | (!!(r & 0x000004) << 16)
    | (!!(r & 0x800000) << 15) | (!!(r & 0x400000) << 14)
    | (!!(r & 0x200000) << 13) | (!!(r & 0x100000) << 12)
    | (!!(r & 0x000002) << 11) | (!!(r & 0x000001) << 10)
    | (!!(r & 0x008000) <<  9) | (!!(r & 0x004000) <<  8)
    | (!!(r & 0x080000) <<  7) | (!!(r & 0x040000) <<  6)
    | (!!(r & 0x020000) <<  5) | (!!(r & 0x010000) <<  4)
    | (!!(r & 0x000200) <<  3) | (!!(r & 0x000100) <<  2)
    | (!!(r & 0x000080) <<  1) | (!!(r & 0x000040) <<  0);
    uint data = r >> 24;
    code = {hex(address, 6L), "=", hex(data, 2L)};
    return true;
  }

  //Pro Action Replay
  if(code.size() == 8) {
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //decode
    uint32_t r = toHex(code);
    uint address = r >> 8;
    uint data = r & 0xff;
    code = {hex(address, 6L), "=", hex(data, 2L)};
    return true;
  }

  //higan: address=data
  if(code.size() == 9 && code[6u] == '=') {
    string nibbles = {code.slice(0, 6), code.slice(7, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //higan: address=compare?data
  if(code.size() == 12 && code[6u] == '=' && code[9u] == '?') {
    string nibbles = {code.slice(0, 6), code.slice(7, 2), code.slice(10, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //unrecognized code format
  return false;
}

auto CheatEditor::decodeGB(string& code) -> bool {
  auto nibble = [&](const string& s, uint index) -> uint {
    if(index >= s.size()) return 0;
    if(s[index] >= '0' && s[index] <= '9') return s[index] - '0';
    return s[index] - 'a' + 10;
  };

  //Game Genie
  if(code.size() == 7 && code[3u] == '-') {
    code = {code.slice(0, 3), code.slice(4, 3)};
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    uint data = nibble(code, 0) << 4 | nibble(code, 1) << 0;
    uint address = (nibble(code, 5) ^ 15) << 12 | nibble(code, 2) << 8 | nibble(code, 3) << 4 | nibble(code, 4) << 0;
    code = {hex(address, 4L), "=", hex(data, 2L)};
    return true;
  }

  //Game Genie
  if(code.size() == 11 && code[3u] == '-' && code[7u] == '-') {
    code = {code.slice(0, 3), code.slice(4, 3), code.slice(8, 3)};
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    uint data = nibble(code, 0) << 4 | nibble(code, 1) << 0;
    uint address = (nibble(code, 5) ^ 15) << 12 | nibble(code, 2) << 8 | nibble(code, 3) << 4 | nibble(code, 4) << 0;
    uint8_t t = nibble(code, 6) << 4 | nibble(code, 8) << 0;
    t = t >> 2 | t << 6;
    uint compare = t ^ 0xba;
    code = {hex(address, 4L), "=", hex(compare, 2L), "?", hex(data, 2L)};
    return true;
  }

  //GameShark
  if(code.size() == 8) {
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //first two characters are the code type / VRAM bank, which is almost always 01.
    //other values are presumably supported, but I have no info on them, so they're not supported.
    if(code[0u] != '0') return false;
    if(code[1u] != '1') return false;
    uint data = toHex(code.slice(2, 2));
    uint16_t address = toHex(code.slice(4, 4));
    address = address >> 8 | address << 8;
    code = {hex(address, 4L), "=", hex(data, 2L)};
    return true;
  }

  //higan: address=data
  if(code.size() == 7 && code[4u] == '=') {
    string nibbles = {code.slice(0, 4), code.slice(5, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //higan: address=compare?data
  if(code.size() == 10 && code[4u] == '=' && code[7u] == '?') {
    string nibbles = {code.slice(0, 4), code.slice(5, 2), code.slice(8, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //unrecognized code format
  return false;
}
