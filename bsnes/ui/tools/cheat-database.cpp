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
  layout.append(cheatList, ~0, ~0, 5);
  layout.append(controlLayout, ~0, 0);
    controlLayout.append(selectAllButton, 100, 0, 5);
    controlLayout.append(unselectAllButton, 100, 0);
    controlLayout.append(spacer, ~0, 0);
    controlLayout.append(acceptButton, 80, 0);

  selectAllButton.onTick = [&] {
    foreach(item, cheatCode, n) cheatList.setChecked(n, true);
  };

  unselectAllButton.onTick = [&] {
    foreach(item, cheatCode, n) cheatList.setChecked(n, false);
  };

  acceptButton.onTick = { &CheatDatabase::addCodes, this };
}

void CheatDatabase::findCodes() {
  string data;
  data.readfile({ application->userpath, "cheats.xml" });
  if(auto position = data.position(interface->sha256())) {
    auto startPosition = strpos((const char*)data + position(), ">");
    auto endPosition = strpos((const char*)data + position(), "</cartridge>");
    string xmlData = {
      "<cartridge>\n",
      substr((const char*)data + position() + 1, startPosition(), endPosition() - startPosition() - 1),
      "</cartridge>\n"
    };

    setTitle("");
    cheatList.reset();
    cheatCode.reset();

    xml_element document = xml_parse(xmlData);
    foreach(root, document.element) {
      if(root.name == "cartridge") {
        foreach(node, root.element) {
          if(node.name == "name") {
            setTitle(node.parse());
          } else if(node.name == "cheat") {
            string description, code;
            foreach(element, node.element) {
              if(element.name == "description") {
                description = element.parse();
              } else if(element.name == "code") {
                code.append(element.parse(), "+");
              }
            }
            code.rtrim<1>("+");
            code.append("\t");
            code.append(description);
            cheatList.append(description);
            cheatCode.append(code);
          }
        }
      }
    }

    setVisible();
  } else {
    MessageWindow::information(*cheatEditor, "Sorry, no cheat codes were found for this cartridge.");
  }
}

void CheatDatabase::addCodes() {
  foreach(code, cheatCode, n) {
    if(cheatList.checked(n)) {
      lstring part;
      part.split<1>("\t", code);
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
