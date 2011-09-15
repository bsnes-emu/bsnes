CheatEditor *cheatEditor = 0;

CheatEditor::CheatEditor() {
  setTitle("Cheat Editor");
  setGeometry({ 128, 128, 600, 360 });

  cheatList.setHeaderText("Slot", "Code", "Description");
  cheatList.setHeaderVisible();
  cheatList.setCheckable();
  codeLabel.setText("Code(s):");
  descLabel.setText("Description:");
  findButton.setText("Find Codes ...");
  findButton.setEnabled(false);
  clearAllButton.setText("Clear All");
  clearButton.setText("Clear");

  append(layout);
  layout.setMargin(5);
  layout.append(cheatList, ~0, ~0, 5);
  layout.append(codeLayout, ~0, 0, 5);
    codeLayout.append(codeLabel, 80, 0);
    codeLayout.append(codeEdit, ~0, 0);
  layout.append(descLayout, ~0, 0, 5);
    descLayout.append(descLabel, 80, 0);
    descLayout.append(descEdit, ~0, 0);
  layout.append(controlLayout, ~0, 0);
    controlLayout.append(findButton, 100, 0);
    controlLayout.append(spacer, ~0, 0);
    controlLayout.append(clearAllButton, 80, 0, 5);
    controlLayout.append(clearButton, 80, 0);

  for(unsigned n = 0; n < 128; n++) cheatList.append("", "", "");
  updateUI();
  synchronize();

  cheatList.onChange = { &CheatEditor::synchronize, this };
  cheatList.onTick = [&](unsigned) { updateInterface(); };
  codeEdit.onChange = { &CheatEditor::updateCode, this };
  descEdit.onChange = { &CheatEditor::updateDesc, this };
  clearAllButton.onTick = { &CheatEditor::clearAll, this };
  clearButton.onTick = { &CheatEditor::clearSelected, this };
}

void CheatEditor::synchronize() {
  layout.setEnabled(interface->loaded());

  if(cheatList.selected()) {
    unsigned n = cheatList.selection();
    codeEdit.setText(cheatText[n][Code]);
    descEdit.setText(cheatText[n][Desc]);
    codeEdit.setEnabled(true);
    descEdit.setEnabled(true);
    clearButton.setEnabled(true);
  } else {
    codeEdit.setText("");
    codeEdit.setEnabled(false);
    descEdit.setText("");
    descEdit.setEnabled(false);
    clearButton.setEnabled(false);
  }
}

void CheatEditor::updateUI() {
  for(unsigned n = 0; n < 128; n++) {
    string code = cheatText[n][Code];
    string description = cheatText[n][Code] == "" && cheatText[n][Desc] == "" ? "(empty)" : cheatText[n][Desc];
    lstring codes;
    codes.split("+", code);
    cheatList.modify(n, decimal<3>(n + 1), codes.size() == 1 ? code : string{ codes[0], "+..." }, description);
  }
  cheatList.autoSizeColumns();
}

void CheatEditor::updateInterface() {
  lstring cheatCodes;
  for(unsigned n = 0; n < 128; n++) {
    string code = cheatText[n][Code];
    if(cheatList.checked(n) && code != "") cheatCodes.append(code.replace(" ", ""));
  }
  interface->setCheatCodes(cheatCodes);
}

void CheatEditor::updateCode() {
  unsigned n = cheatList.selection();
  cheatText[n][Code] = codeEdit.text();
  updateUI(), updateInterface();
}

void CheatEditor::updateDesc() {
  unsigned n = cheatList.selection();
  cheatText[n][Desc] = descEdit.text();
  updateUI(), updateInterface();
}

void CheatEditor::clearAll() {
  if(MessageWindow::question(*this, "Warning: all cheat codes will be permanently erased!\nAre you sure?")
  == MessageWindow::Response::Yes) {
    reset();
  }
}

void CheatEditor::clearSelected() {
  unsigned n = cheatList.selection();
  cheatList.setChecked(n, false);
  cheatText[n][Code] = "";
  cheatText[n][Desc] = "";
  codeEdit.setText("");
  descEdit.setText("");
  updateUI(), updateInterface();
}

void CheatEditor::reset() {
  synchronize();
  for(unsigned n = 0; n < 128; n++) {
    cheatList.setChecked(n, false);
    cheatText[n][Code] = "";
    cheatText[n][Desc] = "";
  }
  codeEdit.setText("");
  descEdit.setText("");
  updateUI(), updateInterface();
}

bool CheatEditor::load(const string &filename) {
  synchronize();

  string data;
  if(data.readfile(filename) == false) return false;

  unsigned n = 0;
  xml_element document = xml_parse(data);
  foreach(head, document.element) {
    if(head.name == "cartridge") {
      foreach(node, head.element) {
        if(node.name == "cheat") {
          bool enable = false;
          string description;
          string code;
          foreach(attribute, node.attribute) {
            if(attribute.name == "enabled") enable = (attribute.parse() == "true");
          }
          foreach(element, node.element) {
            if(element.name == "description") description = element.parse();
            else if(element.name == "code") code.append(element.parse(), "+");
          }
          code.rtrim<1>("+");

          cheatList.setChecked(n, enable);
          cheatText[n][Code] = code;
          cheatText[n][Desc] = description;

          if(++n >= 128) break;
        }
      }
    }
  }

  updateUI(), updateInterface();
  return true;
}

bool CheatEditor::save(const string &filename) {
  synchronize();

  signed lastSave = -1;
  for(signed n = 127; n >= 0; n--) {
    if(cheatText[n][Code] != "" || cheatText[n][Desc] != "") {
      lastSave = n;
      break;
    }
  }

  if(lastSave == -1) {
    unlink(filename);
    return true;
  }

  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;

  fp.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fp.print("<cartridge>\n");
  for(unsigned n = 0; n <= lastSave; n++) {
    fp.print("  <cheat enabled=\"", cheatList.checked(n) ? "true" : "false", "\">\n");
    fp.print("    <description>", cheatText[n][Desc], "</description>\n");
    lstring list;
    list.split("+", cheatText[n][Code]);
    foreach(code, list) {
      fp.print("    <code>", code, "</code>\n");
    }
    fp.print("  </cheat>\n");
  }
  fp.print("</cartridge>\n");
  fp.close();

  return true;
}
