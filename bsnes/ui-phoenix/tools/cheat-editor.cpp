CheatEditor cheatEditor;

void CheatEditor::load(string filename) {
  SNES::cheat.reset();
  cheatList.reset();
  for(unsigned i = 0; i < 128; i++) {
    cheatList.addItem("");
    cheatText[i][CheatSlot] = decimal<3, ' '>(i + 1);
    cheatText[i][CheatCode] = "";
    cheatText[i][CheatDesc] = "";
  }

  unsigned n = 0;
  string data;
  data.readfile(string(filename, ".cht"));
  xml_element document = xml_parse(data);
  foreach(head, document.element) {
    if(head.name == "cartridge") {
      foreach(node, head.element) {
        if(node.name == "cheat") {
          bool enabled = false;
          string description;
          string code;
          foreach(attribute, node.attribute) {
            if(attribute.name == "enabled") enabled = (attribute.parse() == "true");
          }
          foreach(element, node.element) {
            if(element.name == "description") description = element.parse();
            else if(element.name == "code") code.append(string(element.parse(), "+"));
          }
          code.rtrim("+");
          SNES::cheat[n].enabled = enabled;
          SNES::cheat[n] = code;
          cheatText[n][CheatCode] = code;
          cheatText[n][CheatDesc] = description;
          if(++n >= 128) break;
        }
      }
    }
  }

  refresh();
  synchronize();
}

void CheatEditor::save(string filename) {
  signed lastSave = -1;
  for(signed i = 127; i >= 0; i--) {
    if(cheatText[i][CheatCode] != "" || cheatText[i][CheatDesc] != "") {
      lastSave = i;
      break;
    }
  }
  if(lastSave == -1) {
    unlink(string(filename, ".cht"));
    return;
  }

  file fp;
  if(fp.open(string(filename, ".cht"), file::mode::write)) {
    fp.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fp.print(string("<cartridge sha256=\"", SNES::cartridge.sha256(), "\">\n"));
    for(unsigned i = 0; i <= lastSave; i++) {
      fp.print(string("  <cheat enabled=\"", cheatList.checked(i), "\">\n"));
      fp.print(string("    <description><![CDATA[", cheatText[i][CheatDesc], "]]></description>\n"));
      lstring list;
      list.split("+", cheatText[i][CheatCode]);
      foreach(code, list) {
        fp.print(string("    <code>", code, "</code>\n"));
      }
      fp.print(string("  </cheat>\n"));
    }
    fp.print("</cartridge>\n");
    fp.close();
  }

  cheatList.reset();
  cheatList.resizeColumnsToContent();
}

void CheatEditor::create() {
  Window::create(0, 0, 256, 256, "Cheat Editor");
  application.addWindow(this, "CheatEditor", "160,160");

  unsigned x = 5, y = 5, height = Style::ButtonHeight;

  cheatList.create(*this, x,      y, 500, 250, "Slot\tCode\tDescription"); y += 255;
  cheatList.setHeaderVisible();
  cheatList.setCheckable();

  codeLabel.create(*this, x,      y,  80, Style::TextBoxHeight, "Code(s):");
  codeEdit.create (*this, x + 80, y, 420, Style::TextBoxHeight); y += Style::TextBoxHeight + 5;

  descLabel.create(*this, x,      y,  80, Style::TextBoxHeight, "Description:");
  descEdit.create (*this, x + 80, y, 420, Style::TextBoxHeight); y+= Style::TextBoxHeight + 5;

  findButton.create(*this, x, y, 100, height, "Find Codes ...");
  clearAllButton.create(*this, x + 505 - 85 - 85, y, 80, height, "Clear All");
  clearButton.create(*this, x + 505 - 85, y, 80, height, "Clear"); y += height + 5;

  setGeometry(0, 0, 510, y);
  synchronize();

  cheatList.onChange = { &CheatEditor::synchronize, this };
  cheatList.onTick = { &CheatEditor::toggle, this };
  codeEdit.onChange = descEdit.onChange = { &CheatEditor::bind, this };
  findButton.onTick = { &CheatEditor::findCodes, this };
  clearAllButton.onTick = { &CheatEditor::clearAll, this };
  clearButton.onTick = { &CheatEditor::clear, this };

  onClose = []() {
    cheatEditor.databaseWindow.setVisible(false);
    return true;
  };

  //databaseWindow
  databaseWindow.create(0, 0, 256, 256);
  application.addWindow(&databaseWindow, "CheatDatabase", "192,192");

  x = 5, y = 5;

  databaseList.create(databaseWindow, x, y, 600, 360); y += 365;
  databaseList.setCheckable(true);

  databaseSelectAll.create(databaseWindow, x, y, 100, height, "Select All");
  databaseUnselectAll.create(databaseWindow, x + 105, y, 100, height, "Unselect All");
  databaseOk.create(databaseWindow, 605 - 80, y, 80, height, "Ok"); y += height + 5;

  databaseWindow.setGeometry(0, 0, 610, y);

  databaseSelectAll.onTick = []() {
    for(unsigned i = 0; i < cheatEditor.databaseCode.size(); i++) {
      cheatEditor.databaseList.setChecked(i, true);
    }
  };

  databaseUnselectAll.onTick = []() {
    for(unsigned i = 0; i < cheatEditor.databaseCode.size(); i++) {
      cheatEditor.databaseList.setChecked(i, false);
    }
  };

  databaseOk.onTick = { &CheatEditor::addDatabaseCodes, this };
}

void CheatEditor::synchronize() {
  findButton.setEnabled(SNES::cartridge.loaded());
  clearAllButton.setEnabled(SNES::cartridge.loaded());
  if(auto position = cheatList.selection()) {
    codeEdit.setText(cheatText[position()][1]);
    descEdit.setText(cheatText[position()][2]);
    codeEdit.setEnabled(true);
    descEdit.setEnabled(true);
    clearButton.setEnabled(true);
  } else {
    codeEdit.setText("");
    descEdit.setText("");
    codeEdit.setEnabled(false);
    descEdit.setEnabled(false);
    clearButton.setEnabled(false);
  }
}

void CheatEditor::refresh() {
  SNES::cheat.synchronize();
  for(unsigned i = 0; i < 128; i++) {
    lstring list;
    list.split("+", cheatText[i][CheatCode]);
    string cheatCode = list[0];
    if(list.size() > 1) cheatCode.append("...");

    cheatList.setChecked(i, SNES::cheat[i].enabled);
    cheatList.setItem(i, {
      cheatText[i][CheatSlot], "\t", cheatCode, "\t", cheatText[i][CheatDesc]
    });
  }
  cheatList.resizeColumnsToContent();
}

void CheatEditor::toggle(unsigned row) {
  SNES::cheat[row].enabled = cheatList.checked(row);
  refresh();
}

void CheatEditor::bind() {
  if(auto position = cheatList.selection()) {
    cheatText[position()][CheatCode] = codeEdit.text();
    cheatText[position()][CheatDesc] = descEdit.text();
    SNES::cheat[position()] = cheatText[position()][CheatCode];
    refresh();
  }
}

void CheatEditor::findCodes() {
  string data;
  data.readfile(string(config.path.user, "cheats.xml"));
  if(data == "") data.readfile(string(config.path.base, "cheats.xml"));
  if(auto position = strpos(data, SNES::cartridge.sha256())) {
    auto startPosition = strpos((const char*)data + position(), ">");
    auto endPosition = strpos((const char*)data + position(), "</cartridge>");
    string xmlData = {
      "<cartridge>\n",
      substr((const char*)data + position() + 1, startPosition(), endPosition() - startPosition() - 1),
      "</cartridge>\n"
    };

    databaseWindow.setTitle("");
    databaseList.reset();
    databaseCode.reset();

    xml_element document = xml_parse(xmlData);
    foreach(root, document.element) {
      if(root.name == "cartridge") foreach(node, root.element) {
        if(node.name == "name") databaseWindow.setTitle(node.parse());
        else if(node.name == "cheat") {
          string description, code;
          foreach(element, node.element) {
            if(element.name == "description") description = element.parse();
            else if(element.name == "code") code.append(string(element.parse(), "+"));
          }
          code.rtrim<1>("+");
          code.append("\t");
          code.append(description);
          databaseList.addItem(description);
          databaseCode.append(code);
        }
      }
    }

    databaseWindow.setVisible(true);
  } else {
    MessageWindow::information(cheatEditor, "Sorry, no cheat codes found for this cartridge.");
  }
}

optional<unsigned> CheatEditor::findUnusedSlot() {
  for(unsigned i = 0; i < 128; i++) {
    if(cheatText[i][CheatCode] == "" && cheatText[i][CheatDesc] == "") return { true, i };
  }
  return { false, 0 };
}

void CheatEditor::addDatabaseCodes() {
  for(unsigned n = 0; n < databaseCode.size(); n++) {
    if(databaseList.checked(n)) {
      if(auto position = findUnusedSlot()) {
        lstring part;
        part.split("\t", databaseCode[n]);
        SNES::cheat[position()].enabled = false;
        SNES::cheat[position()] = part[0];
        cheatList.setChecked(position(), false);
        cheatText[position()][CheatCode] = part[0];
        cheatText[position()][CheatDesc] = part[1];
      } else {
        MessageWindow::warning(databaseWindow, "Ran out of empty slots for cheat codes.\nNot all cheat codes were added.");
        break;
      }
    }
  }
  databaseWindow.setVisible(false);
  refresh();
  synchronize();
}

void CheatEditor::clearAll() {
  if(MessageWindow::question(cheatEditor, "Permanently erase all entered cheat codes?", MessageWindow::Buttons::YesNo) == MessageWindow::Response::Yes) {
    for(unsigned i = 0; i < 128; i++) {
      SNES::cheat[i].enabled = false;
      SNES::cheat[i] = "";
      cheatList.setChecked(i, false);
      cheatText[i][CheatCode] = "";
      cheatText[i][CheatDesc] = "";
    }
    SNES::cheat.synchronize();
    refresh();
    codeEdit.setText("");
    descEdit.setText("");
  }
}

void CheatEditor::clear() {
  if(auto position = cheatList.selection()) {
    SNES::cheat[position()].enabled = false;
    SNES::cheat[position()] = "";
    cheatList.setChecked(position(), false);
    cheatText[position()][CheatCode] = "";
    cheatText[position()][CheatDesc] = "";
    SNES::cheat.synchronize();
    refresh();
    codeEdit.setText("");
    descEdit.setText("");
  }
}
