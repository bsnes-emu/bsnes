CheatEditor *cheatEditor = nullptr;

CheatEditor::CheatEditor() {
  setGeometry({128, 128, 600, 360});
  windowManager->append(this, "CheatEditor");

  setTitle("Cheat Editor");
  layout.setMargin(5);
  cheatList.setHeaderText("Slot", "Code", "Description");
  cheatList.setHeaderVisible();
  cheatList.setCheckable();
  for(unsigned n = 0; n < Codes; n++) cheatList.append("", "", "");
  codeLabel.setText("Code(s):");
  descLabel.setText("Description:");
  findButton.setText("Find Codes ...");
  resetButton.setText("Reset");
  eraseButton.setText("Erase");
  unsigned width = max(
    Font(application->normalFont).geometry("Codes(s)"    ).width,
    Font(application->normalFont).geometry("Description:").width
  );

  append(layout);
  layout.append(cheatList, {~0, ~0}, 5);
  layout.append(codeLayout, {~0, 0}, 5);
    codeLayout.append(codeLabel, {width, 0}, 5);
    codeLayout.append(codeEdit, {~0, 0});
  layout.append(descLayout, {~0, 0}, 5);
    descLayout.append(descLabel, {width, 0}, 5);
    descLayout.append(descEdit, {~0, 0});
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(findButton, {0, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(resetButton, {80, 0}, 5);
    controlLayout.append(eraseButton, {80, 0});

  cheatList.onChange = {&CheatEditor::synchronize, this};
  cheatList.onToggle = [&](unsigned) { update(); };
  codeEdit.onChange = {&CheatEditor::updateCode, this};
  descEdit.onChange = {&CheatEditor::updateDesc, this};
  findButton.onActivate = {&CheatDatabase::findCodes, cheatDatabase};
  resetButton.onActivate = [&] {
    if(MessageWindow::question(*this, "All codes will be erased. Are you sure you want to do this?")
    == MessageWindow::Response::Yes) reset();
  };
  eraseButton.onActivate = {&CheatEditor::erase, this};

  cheatList.setSelection(0);
  synchronize();
}

void CheatEditor::synchronize() {
  layout.setEnabled(application->active);

  if(cheatList.selected()) {
    unsigned n = cheatList.selection();
    codeEdit.setText(cheat[n].code);
    descEdit.setText(cheat[n].desc);
    codeEdit.setEnabled(true);
    descEdit.setEnabled(true);
    eraseButton.setEnabled(true);
  } else {
    codeEdit.setText("");
    descEdit.setText("");
    codeEdit.setEnabled(false);
    descEdit.setEnabled(false);
    eraseButton.setEnabled(false);
  }
}

void CheatEditor::refresh() {
  for(unsigned n = 0; n < Codes; n++) {
    string code = cheat[n].code;
    string desc = cheat[n].code.empty() && cheat[n].desc.empty() ? "(empty)" : cheat[n].desc;
    lstring codes = code.split("+");
    if(codes.size() > 1) code = {codes[0], "+..."};
    cheatList.modify(n, decimal<3>(1 + n), code, desc);
  }
  cheatList.autoSizeColumns();
}

void CheatEditor::update() {
  lstring codes;
  for(unsigned n = 0; n < Codes; n++) {
    string code = cheat[n].code;
    if(cheatList.checked(n) && !code.empty()) codes.append(code);
  }
  system().cheatSet(codes);
}

void CheatEditor::reset() {
  for(unsigned n = 0; n < Codes; n++) {
    cheatList.setChecked(n, false);
    cheat[n].code = "";
    cheat[n].desc = "";
  }
  codeEdit.setText("");
  descEdit.setText("");
  refresh();
  update();
}

void CheatEditor::erase() {
  unsigned n = cheatList.selection();
  cheatList.setChecked(n, false);
  cheat[n].code = "";
  cheat[n].desc = "";
  codeEdit.setText("");
  descEdit.setText("");
  refresh();
  update();
}

void CheatEditor::updateCode() {
  unsigned n = cheatList.selection();
  cheat[n].code = codeEdit.text();
  refresh();
  update();
}

void CheatEditor::updateDesc() {
  unsigned n = cheatList.selection();
  cheat[n].desc = descEdit.text();
  refresh();
}

bool CheatEditor::load(const string &filename) {
  string data;
  if(data.readfile(filename) == false) return false;

  unsigned n = 0;
  XML::Document document(data);
  for(auto &node : document["cartridge"]) {
    if(node.name != "cheat") continue;
    cheatList.setChecked(n, node["enable"].data == "true");
    cheat[n].code = node["code"].data;
    cheat[n].desc = node["description"].data;
    if(++n >= Codes) break;
  }

  refresh();
  update();
  return true;
}

bool CheatEditor::save(const string &filename) {
  signed lastSave = -1;
  for(signed n = 127; n >= 0; n--) {
    if(!cheat[n].code.empty() || !cheat[n].desc.empty()) {
      lastSave = n;
      break;
    }
  }

  if(lastSave == -1) {
    file::remove(filename);
    return true;
  }

  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;

  fp.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fp.print("<cartridge sha256=\"", system().sha256(), "\">\n");
  for(unsigned n = 0; n <= lastSave; n++) {
    fp.print("  <cheat enable=\"", cheatList.checked(n) ? "true" : "false", "\">\n");
    fp.print("    <description><![CDATA[", cheat[n].desc, "]]></description>\n");
    fp.print("    <code><![CDATA[", cheat[n].code, "]]></code>\n");
    fp.print("  </cheat>\n");
  }
  fp.print("</cartridge>\n");
  fp.close();

  return true;
}

bool CheatEditor::import(const string &code, const string &desc) {
  for(unsigned n = 0; n < Codes; n++) {
    if(cheat[n].code.empty() && cheat[n].desc.empty()) {
      cheatList.setChecked(n, false);
      cheat[n].code = code;
      cheat[n].desc = desc;
      return true;
    }
  }
  return false;
}
