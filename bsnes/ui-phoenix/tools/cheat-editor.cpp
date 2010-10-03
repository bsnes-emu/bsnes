CheatEditor cheatEditor;

void CheatEditor::load(string filename) {
  SNES::cheat.reset();
  cheatList.reset();
  for(unsigned i = 0; i < 128; i++) {
    cheatList.addItem("");
    cheatText[i][0] = strunsigned<3, ' '>(i + 1);
    cheatText[i][1] = " ";
    cheatText[i][2] = "";
    cheatText[i][3] = "";
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
          cheatText[n][1] = (enabled == false ? " " : "*");
          cheatText[n][2] = code;
          cheatText[n][3] = description;
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
    if(cheatText[i][2] != "" || cheatText[i][3] != "") {
      lastSave = i;
      break;
    }
  }
  if(lastSave == -1) {
    unlink(string(filename, ".cht"));
    return;
  }

  file fp;
  if(fp.open(string(filename, ".cht"), file::mode_write)) {
    fp.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fp.print(string("<cartridge sha256=\"", SNES::cartridge.sha256(), "\">\n"));
    for(unsigned i = 0; i <= lastSave; i++) {
      fp.print(string("  <cheat enabled=\"", cheatText[i][1] == " " ? "false" : "true", "\">\n"));
      fp.print(string("    <description><![CDATA[", cheatText[i][3], "]]></description>\n"));
      lstring list;
      list.split("+", cheatText[i][2]);
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
  application.windows.append(this);
  Window::create(0, 0, 256, 256, "Cheat Editor");
  setDefaultFont(application.proportionalFont);

  unsigned x = 5, y = 5, height = Style::ButtonHeight;

  cheatList.create(*this, x,      y, 500, 250, "Slot\tCode\tDescription"); y += 255;
  cheatList.setHeaderVisible();

  codeLabel.create(*this, x,      y,  80, Style::TextBoxHeight, "Code(s):");
  codeEdit.create (*this, x + 80, y, 420, Style::TextBoxHeight); y += Style::TextBoxHeight + 5;

  descLabel.create(*this, x,      y,  80, Style::TextBoxHeight, "Description:");
  descEdit.create (*this, x + 80, y, 420, Style::TextBoxHeight); y+= Style::TextBoxHeight + 5;

  clearAllButton.create(*this, x + 505 - 85 - 85, y, 80, height, "Clear All");
  clearButton.create(*this, x + 505 - 85, y, 80, height, "Clear"); y += height + 5;

  setGeometry(160, 160, 510, y);
  synchronize();

  cheatList.onActivate = { &CheatEditor::toggle, this };
  cheatList.onChange = { &CheatEditor::synchronize, this };
  codeEdit.onChange = descEdit.onChange = { &CheatEditor::bind, this };
  clearAllButton.onTick = { &CheatEditor::clearAll, this };
  clearButton.onTick = { &CheatEditor::clear, this };
}

void CheatEditor::synchronize() {
  clearAllButton.setEnabled(SNES::cartridge.loaded());
  if(auto position = cheatList.selection()) {
    codeEdit.setText(cheatText[position()][2]);
    descEdit.setText(cheatText[position()][3]);
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
    list.split("+", cheatText[i][2]);
    string cheatCode = list[0];
    if(list.size() > 1) cheatCode.append("...");

    cheatList.setItem(i, string(
      cheatText[i][0], cheatText[i][1], "\t", cheatCode, "\t", cheatText[i][3]
    ));
  }
  cheatList.resizeColumnsToContent();
}

void CheatEditor::toggle() {
  if(auto position = cheatList.selection()) {
    if(cheatText[position()][1] == " ") {
      cheatText[position()][1] = "*";
      SNES::cheat[position()].enabled = true;
    } else {
      cheatText[position()][1] = " ";
      SNES::cheat[position()].enabled = false;
    }
  }
  refresh();
}

void CheatEditor::bind() {
  if(auto position = cheatList.selection()) {
    cheatText[position()][2] = codeEdit.text();
    cheatText[position()][3] = descEdit.text();
    SNES::cheat[position()] = cheatText[position()][2];
    refresh();
  }
}

void CheatEditor::clearAll() {
  if(MessageWindow::question(cheatEditor, "Permanently erase all entered cheat codes?", MessageWindow::Buttons::YesNo) == MessageWindow::Response::Yes) {
    for(unsigned i = 0; i < 128; i++) {
      SNES::cheat[i].enabled = false;
      SNES::cheat[i] = "";
      cheatText[i][1] = " ";
      cheatText[i][2] = "";
      cheatText[i][3] = "";
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
    cheatText[position()][1] = " ";
    cheatText[position()][2] = "";
    cheatText[position()][3] = "";
    SNES::cheat.synchronize();
    refresh();
    codeEdit.setText("");
    descEdit.setText("");
  }
}
