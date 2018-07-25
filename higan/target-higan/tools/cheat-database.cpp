CheatDatabase::CheatDatabase() {
  cheatDatabase = this;

  layout.setPadding(5);
  selectAllButton.setText("Select All").onActivate([&] {
    for(auto& item : cheatList.items()) item.setChecked(true);
  });
  unselectAllButton.setText("Unselect All").onActivate([&] {
    for(auto& item : cheatList.items()) item.setChecked(false);
  });
  addCodesButton.setText("Add Codes").onActivate([&] { addCodes(); });

  setSize({800, 400});
  setAlignment({0.5, 1.0});
  setDismissable();
}

auto CheatDatabase::findCodes() -> void {
  if(!emulator) return;
  auto sha256 = emulator->hashes().left();

  auto contents = string::read(locate("cheats.bml"));
  auto document = BML::unserialize(contents);

  for(auto cartridge : document.find("cartridge")) {
    if(cartridge["sha256"].text() != sha256) continue;

    codes.reset();
    cheatList.reset();
    for(auto cheat : cartridge.find("cheat")) {
      codes.append(cheat["code"].text());
      cheatList.append(ListViewItem().setCheckable().setText(cheat["description"].text()));
    }

    setTitle(cartridge["name"].text());
    setVisible();
    return;
  }

  MessageDialog().setParent(*toolsManager).setText("Sorry, no cheats were found for this game.").information();
}

auto CheatDatabase::addCodes() -> void {
  for(auto& item : cheatList.items()) {
    if(!item.checked()) continue;

    string code = codes(item.offset(), "");
    string description = item.text();
    if(toolsManager->cheatEditor.addCode(code, description) == false) {
      MessageDialog().setParent(*this).setText("Free slots exhausted. Not all codes could be added.").warning();
      break;
    }
  }
  setVisible(false);
  toolsManager->cheatEditor.doRefresh();
}
