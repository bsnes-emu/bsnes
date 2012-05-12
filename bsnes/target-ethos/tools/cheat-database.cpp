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

  string data;
  data.readfile(application->path("cheats.xml"));
  XML::Document document(data);
  for(auto &node : document["database"]) {
    if(node.name != "cartridge") continue;
    if(node["sha256"].data != sha256) continue;

    setTitle(node["name"].data);
    for(auto &cheat : node) {
      if(cheat.name != "cheat") continue;
      cheatList.append(cheat["description"].data);
      this->cheat.append({cheat["code"].data, cheat["description"].data});
    }

    setVisible();
    return;
  }

  MessageWindow::information(*cheatEditor, "Sorry, no cheat codes were found.");
}

void CheatDatabase::addCodes() {
  for(unsigned n = 0; n < cheat.size(); n++) {
    if(cheatList.checked(n) == false) continue;
    if(cheatEditor->import(cheat[n].code, cheat[n].desc) == false) {
      MessageWindow::warning(*this, "Ran out of empty slots for cheat codes.\nNot all cheat codes were added.");
      break;
    }
  }

  setVisible(false);
  cheatEditor->synchronize();
  cheatEditor->refresh();
}
