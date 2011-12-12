CheatDatabase *cheatDatabase = 0;

CheatDatabase::CheatDatabase() {
  setGeometry({ 128, 128, 640, 400 });
  windowManager->append(this, "CheatDatabase");

  layout.setMargin(5);
  cheatList.setCheckable();
  selectAllButton.setText("Select All");
  unselectAllButton.setText("Unselect All");
  acceptButton.setText("Add Codes");

  append(layout);
  layout.append(cheatList,                  {  ~0, ~0 }, 5);
  layout.append(controlLayout,              {  ~0,  0 }, 0);
    controlLayout.append(selectAllButton,   { 100,  0 }, 5);
    controlLayout.append(unselectAllButton, { 100,  0 }, 0);
    controlLayout.append(spacer,            {  ~0,  0 }, 0);
    controlLayout.append(acceptButton,      {  80,  0 }, 0);

  selectAllButton.onActivate = [&] {
    for(unsigned n = 0; n < cheatCode.size(); n++) cheatList.setChecked(n, true);
  };

  unselectAllButton.onActivate = [&] {
    for(unsigned n = 0; n < cheatCode.size(); n++) cheatList.setChecked(n, false);
  };

  acceptButton.onActivate = { &CheatDatabase::addCodes, this };
}

void CheatDatabase::findCodes() {
  cheatList.reset();
  cheatCode.reset();

  string data;
  data.readfile(application->path("cheats.bml"));
  BML::Document document(data);
  for(auto &root : document) {
    if(root.name != "cartridge") continue;
    if(root["sha256"].value != interface->sha256()) continue;

    setTitle(root["title"].value);
    for(auto &cheat : root) {
      if(cheat.name != "cheat") continue;
      if(cheat["description"].exists() == false || cheat["code"].exists() == false) continue;
      cheatList.append(cheat["description"].value);
      cheatCode.append({ cheat["code"].value, "\t", cheat["description"].value });
    }

    setVisible();
    return;
  }

  MessageWindow::information(*cheatEditor, "Sorry, no cheat codes were found for this cartridge.");
}

void CheatDatabase::addCodes() {
  for(unsigned n = 0; n < cheatCode.size(); n++) {
    if(cheatList.checked(n)) {
      lstring part;
      part.split<1>("\t", cheatCode[n]);
      if(cheatEditor->addCode(part[0], part[1]) == false) {
        MessageWindow::warning(*this, "Ran out of empty slots for cheat codes.\nNot all cheat codes were added.");
        break;
      }
    }
  }

  setVisible(false);
  cheatEditor->updateUI();
  cheatEditor->updateInterface();
  cheatEditor->synchronize();
}
