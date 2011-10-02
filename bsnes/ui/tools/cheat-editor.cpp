CheatEditor *cheatEditor = 0;

CheatEditor::CheatEditor() {
  setTitle("Cheat Editor");
  setGeometry({ 128, 128, 600, 360 });
  windowManager->append(this, "CheatEditor");

  cheatList.setHeaderText("Slot", "Code", "Description");
  cheatList.setHeaderVisible();
  cheatList.setCheckable();
  codeLabel.setText("Code(s):");
  descLabel.setText("Description:");
  findButton.setText("Find Codes ...");
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
  findButton.onTick = { &CheatDatabase::findCodes, cheatDatabase };
  clearAllButton.onTick = { &CheatEditor::clearAll, this };
  clearButton.onTick = { &CheatEditor::clearSelected, this };
}

void CheatEditor::synchronize() {
  layout.setEnabled(interface->cartridgeLoaded());

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
  BML::Node document(data);
  for(auto &cheat : document["cartridge"]) {
    if(cheat.name != "cheat") continue;
    cheatList.setChecked(n, cheat["enable"].exists());
    cheatText[n][Code] = cheat["code"].value;
    cheatText[n][Desc] = cheat["description"].value;
    if(++n >= 128) break;
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

  fp.print("cartridge sha256{", interface->sha256(), "}\n");
  for(unsigned n = 0; n <= lastSave; n++) {
    fp.print("\tcheat", cheatList.checked(n) ? " enable" : "", "\n");
    fp.print("\t\tdescription{", BML::encode(cheatText[n][Desc]), "}\n");
    fp.print("\t\tcode{", BML::encode(cheatText[n][Code]), "}\n");
  }

  fp.close();
  return true;
}

bool CheatEditor::addCode(const string &code, const string &description) {
  for(unsigned n = 0; n < 128; n++) {
    if(cheatText[n][Code] == "" && cheatText[n][Desc] == "") {
      cheatList.setChecked(n, false);
      cheatText[n][Code] = code;
      cheatText[n][Desc] = description;
      return true;
    }
  }
  return false;
}
