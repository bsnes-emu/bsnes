CheatDatabase cheatDatabase;

void CheatDatabase::create() {
  application.addWindow(this, "CheatDatabase", "192,192");

  listView.setCheckable(true);
  selectAllButton.setText("Select All");
  unselectAllButton.setText("Unselect All");
  okButton.setText("Ok");

  layout.setMargin(5);
  layout.append(listView,                  ~0, ~0, 5);
  controlLayout.append(selectAllButton,   100,  0, 5);
  controlLayout.append(unselectAllButton, 100,  0   );
  controlLayout.append(spacerWidget,       ~0,  0   );
  controlLayout.append(okButton,           80,  0   );
  layout.append(controlLayout                       );
  append(layout);
  setGeometry({ 0, 0, 600, layout.minimumGeometry().height + 350 });

  selectAllButton.onTick = [this]() {
    foreach(item, this->listData, n) this->listView.setChecked(n, true);
  };

  unselectAllButton.onTick = [this]() {
    foreach(item, this->listData, n) this->listView.setChecked(n, false);
  };

  okButton.onTick = { &CheatDatabase::addCodes, this };
}

void CheatDatabase::findCodes() {
  string data;
  data.readfile(path.home("cheats.xml"));
  if(auto position = strpos(data, SNES::cartridge.sha256())) {
    auto startPosition = strpos((const char*)data + position(), ">");
    auto endPosition = strpos((const char*)data + position(), "</cartridge>");
    string xmlData = {
      "<cartridge>\n",
      substr((const char*)data + position() + 1, startPosition(), endPosition() - startPosition() - 1),
      "</cartridge>\n"
    };

    setTitle("");
    listView.reset();
    listData.reset();

    xml_element document = xml_parse(xmlData);
    foreach(root, document.element) {
      if(root.name == "cartridge") foreach(node, root.element) {
        if(node.name == "name") setTitle(node.parse());
        else if(node.name == "cheat") {
          string description, code;
          foreach(element, node.element) {
            if(element.name == "description") description = element.parse();
            else if(element.name == "code") code.append(string(element.parse(), "+"));
          }
          code.rtrim<1>("+");
          code.append("\t");
          code.append(description);
          listView.append(description);
          listData.append(code);
        }
      }
    }

    setVisible(true);
  } else {
    MessageWindow::information(cheatEditor, "Sorry, no cheat codes found for this cartridge.");
  }
}

void CheatDatabase::addCodes() {
  foreach(code, listData, n) {
    if(listView.checked(n)) {
      if(auto position = cheatEditor.findUnusedSlot()) {
        lstring part;
        part.split("\t", code);
        SNES::cheat[position()].enabled = false;
        SNES::cheat[position()] = part[0];
        cheatEditor.cheatList.setChecked(position(), false);
        cheatEditor.cheatText[position()][CheatEditor::CheatCode] = part[0];
        cheatEditor.cheatText[position()][CheatEditor::CheatDesc] = part[1];
      } else {
        MessageWindow::warning(*this, "Ran out of empty slots for cheat codes.\nNot all cheat codes were added.");
        break;
      }
    }
  }
  setVisible(false);
  cheatEditor.refresh();
  cheatEditor.synchronize();
}
