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
    if(!item.checked()) continue;

    string code = item.property("code").replace("/", "=", 1L).replace("/", "?", 1L);
    string description = item.text();
    if(!toolsWindow->cheatEditor.addCode(false, code, description)) {
      MessageDialog().setParent(*this).setText("Free slots exhausted. Not all cheats could be added.").warning();
      break;
    }
  }
  setVisible(false);
  toolsWindow->cheatEditor.doRefresh();
}
