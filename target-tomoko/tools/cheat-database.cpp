CheatDatabase::CheatDatabase() {
  cheatDatabase = this;

  layout.setMargin(5);
  cheatList.setCheckable();
  selectAllButton.setText("Select All").onActivate([&] { cheatList.setChecked(true); });
  unselectAllButton.setText("Unselect All").onActivate([&] { cheatList.setChecked(false); });
  addCodesButton.setText("Add Codes").onActivate([&] { addCodes(); });

  setSize({800, 400});
  setPlacement(0.5, 1.0);
}

auto CheatDatabase::findCodes() -> void {
  if(!emulator) return;
  auto sha256 = emulator->sha256();

  auto contents = string::read({localpath(), "tomoko/cheats.bml"});
  auto document = Markup::Document(contents);

  for(auto& cartridge : document) {
    if(cartridge.name != "cartridge") continue;
    if(cartridge["sha256"].text() != sha256) continue;

    codes.reset();
    cheatList.reset();
    cheatList.append(ListViewColumn().setWidth(~0));
    for(auto& cheat : cartridge) {
      if(cheat.name != "cheat") continue;
      codes.append(cheat["code"].text());
      cheatList.append(ListViewItem().setText(0, cheat["description"].text()));
    }

    setTitle(cartridge["name"].text());
    setVisible();
    return;
  }

  MessageDialog().setParent(*toolsManager).setText("Sorry, no cheats were found for this game.").information();
}

auto CheatDatabase::addCodes() -> void {
  for(auto item : cheatList.checked()) {
    string code = codes(item->offset(), "");
    string description = item->text(0);
    if(toolsManager->cheatEditor.addCode(code, description) == false) {
      MessageDialog().setParent(*this).setText("Free slots exhausted. Not all codes could be added.").warning();
      break;
    }
  }
  setVisible(false);
}
