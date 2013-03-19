CheatDatabase *cheatDatabase = nullptr;

CheatDatabase::CheatDatabase() {
  setGeometry({256, 256, 600, 360});
  windowManager->append(this, "CheatDatabase");

  layout.setMargin(5);
  cheatList.setCheckable();
  selectAllButton.setText("Select All");
  unselectAllButton.setText("Unselect All");
  acceptButton.setText("Add Codes");

  append(layout);
  layout.append(cheatList, {~0, ~0}, 5);
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(selectAllButton, {100, 0}, 5);
    controlLayout.append(unselectAllButton, {100, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(acceptButton, {80, 0});

  selectAllButton.onActivate = [&] {
    for(unsigned n = 0; n < cheat.size(); n++) cheatList.setChecked(n, true);
  };

  unselectAllButton.onActivate = [&] {
    for(unsigned n = 0; n < cheat.size(); n++) cheatList.setChecked(n, false);
  };

  acceptButton.onActivate = {&CheatDatabase::addCodes, this};
}

void CheatDatabase::findCodes() {
  const string sha256 = system().sha256();
  cheatList.reset();
  cheat.reset();

  auto document = Markup::Document(string::read(program->path("cheats.bml")));
  for(auto &node : document) {
    if(node.name != "cartridge") continue;
    if(node["sha256"].text() != sha256) continue;

    setTitle(node["name"].text());
    for(auto &cheat : node) {
      if(cheat.name != "cheat") continue;
      cheatList.append(cheat["description"].text());
      this->cheat.append({cheat["code"].text(), cheat["description"].text()});
    }

    setVisible();
    return;
  }

  MessageWindow().setParent(*cheatEditor).setText("Sorry, no cheat codes were found.").information();
}

void CheatDatabase::addCodes() {
  for(unsigned n = 0; n < cheat.size(); n++) {
    if(cheatList.checked(n) == false) continue;
    if(cheatEditor->import(cheat[n].code, cheat[n].desc) == false) {
      MessageWindow().setParent(*this).setText("Ran out of empty slots for cheat codes.\nNot all cheat codes were added.").warning();
      break;
    }
  }

  setVisible(false);
  cheatEditor->synchronize();
  cheatEditor->refresh();
}
