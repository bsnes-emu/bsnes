CheatEditor cheatEditor;

void CheatEditor::load(string filename) {
  SNES::cheat.reset();
  cheatList.reset();
  for(unsigned i = 0; i < 128; i++) {
    cheatList.append("");
    cheatText[i][CheatSlot] = rdecimal<3>(i + 1);
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
  cheatList.autoSizeColumns();
}

void CheatEditor::create() {
  setTitle("Cheat Editor");
  application.addWindow(this, "CheatEditor", "160,160");

  cheatList.setHeaderText("Slot", "Code", "Description");
  cheatList.setHeaderVisible();
  cheatList.setCheckable();
  codeLabel.setText("Code(s):");
  descLabel.setText("Description:");
  findButton.setText("Find Codes ...");
  clearAllButton.setText("Clear All");
  clearButton.setText("Clear");

  layout.setMargin(5);
  layout.append(cheatList, 0, 0, 5);
  codeLayout.append(codeLabel, 80, 0, 5);
  codeLayout.append(codeEdit,   0, 0);
  layout.append(codeLayout, 0, Style::LineEditHeight, 5);
  descLayout.append(descLabel, 80, 0, 5);
  descLayout.append(descEdit,   0, 0);
  layout.append(descLayout, 0, Style::LineEditHeight, 5);
  controlLayout.append(findButton,    100, 0);
  controlLayout.append(spacerWidget,    0, 0);
  controlLayout.append(clearAllButton, 80, 0, 5);
  controlLayout.append(clearButton,    80, 0);
  layout.append(controlLayout, 0, Style::ButtonHeight);

  setGeometry({ 0, 0, 480, layout.minimumHeight() + 250 });
  append(layout);

  synchronize();

  cheatList.onChange = { &CheatEditor::synchronize, this };
  cheatList.onTick = { &CheatEditor::toggle, this };
  codeEdit.onChange = descEdit.onChange = { &CheatEditor::bind, this };
  findButton.onTick = { &CheatDatabase::findCodes, &cheatDatabase };
  clearAllButton.onTick = { &CheatEditor::clearAll, this };
  clearButton.onTick = { &CheatEditor::clear, this };

  onClose = []() {
    cheatDatabase.setVisible(false);
  };
}

void CheatEditor::synchronize() {
  findButton.setEnabled(SNES::cartridge.loaded());
  clearAllButton.setEnabled(SNES::cartridge.loaded());
  if(cheatList.selected()) {
    codeEdit.setText(cheatText[cheatList.selection()][1]);
    descEdit.setText(cheatText[cheatList.selection()][2]);
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
    cheatList.modify(i, cheatText[i][CheatSlot], cheatCode, cheatText[i][CheatDesc]);
  }
  cheatList.autoSizeColumns();
}

void CheatEditor::toggle(unsigned row) {
  SNES::cheat[row].enabled = cheatList.checked(row);
  refresh();
}

void CheatEditor::bind() {
  if(cheatList.selected() == false) return;
  unsigned selection = cheatList.selection();
  cheatText[selection][CheatCode] = codeEdit.text();
  cheatText[selection][CheatDesc] = descEdit.text();
  SNES::cheat[selection] = cheatText[selection][CheatCode];
  refresh();
}

optional<unsigned> CheatEditor::findUnusedSlot() {
  for(unsigned i = 0; i < 128; i++) {
    if(cheatText[i][CheatCode] == "" && cheatText[i][CheatDesc] == "") return { true, i };
  }
  return { false, 0 };
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
  if(cheatList.selected() == false) return;
  unsigned selection = cheatList.selection();
  SNES::cheat[selection].enabled = false;
  SNES::cheat[selection] = "";
  cheatList.setChecked(selection, false);
  cheatText[selection][CheatCode] = "";
  cheatText[selection][CheatDesc] = "";
  SNES::cheat.synchronize();
  refresh();
  codeEdit.setText("");
  descEdit.setText("");
}
