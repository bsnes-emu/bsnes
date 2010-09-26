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

  string data;
  if(data.readfile(string(filename, ".bsv"))) {
    lstring list;
    list.split("\n", data);
    for(unsigned i = 0; i < 128 && i < list.size(); i++) {
      lstring part;
      part.split("{}", list[i]);
      cheatText[i][1] = part[0];
      cheatText[i][2] = part[1];
      cheatText[i][3] = part[2];
      SNES::cheat[i].enabled = (cheatText[i][1] != " ");
      SNES::cheat[i] = cheatText[i][2];
    }
  }
  refresh();
}

void CheatEditor::save(string filename) {
  bool savesPresent = false;
  for(unsigned i = 0; i < 128; i++) {
    if(cheatText[i][2] != "" || cheatText[i][3] != "") {
      savesPresent = true;
      break;
    }
  }
  if(savesPresent == false) {
    unlink(string(filename, ".bsv"));
    return;
  }

  file fp;
  if(fp.open(string(filename, ".bsv"), file::mode_write)) {
    for(unsigned i = 0; i < 128; i++) {
      fp.print(string(cheatText[i][1], "{}", cheatText[i][2], "{}", cheatText[i][3], "\n"));
    }
    fp.close();
  }

  cheatList.reset();
  cheatList.resizeColumnsToContent();
}

void CheatEditor::create() {
  application.windows.append(this);
  Window::create(0, 0, 256, 256, "Cheat Editor");
  setDefaultFont(application.proportionalFont);

  unsigned x = 5, y = 5;

  cheatList.create(*this, x,      y, 500, 250, "Slot\tOn\tCode\tDescription"); y += 255;
  cheatList.setHeaderVisible();

  codeLabel.create(*this, x,      y,  80, Style::EditBoxHeight, "Code(s):");
  codeEdit.create (*this, x + 80, y, 420, Style::EditBoxHeight); y += Style::EditBoxHeight + 5;

  descLabel.create(*this, x,      y,  80, Style::EditBoxHeight, "Description:");
  descEdit.create (*this, x + 80, y, 420, Style::EditBoxHeight); y+= Style::EditBoxHeight + 5;

  setGeometry(160, 160, 510, y);
  synchronize();

  cheatList.onActivate = { &CheatEditor::toggle, this };
  cheatList.onChange = { &CheatEditor::synchronize, this };
  codeEdit.onChange = descEdit.onChange = { &CheatEditor::bind, this };
}

void CheatEditor::synchronize() {
  if(auto position = cheatList.selection()) {
    codeEdit.setText(cheatText[position()][2]);
    descEdit.setText(cheatText[position()][3]);
    codeEdit.setEnabled(true);
    descEdit.setEnabled(true);
  } else {
    codeEdit.setText("");
    descEdit.setText("");
    codeEdit.setEnabled(false);
    descEdit.setEnabled(false);
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
      cheatText[i][0], "\t", cheatText[i][1], "\t", cheatCode, "\t", cheatText[i][3]
    ));
  }
  cheatList.resizeColumnsToContent();
}

void CheatEditor::toggle() {
  if(auto position = cheatList.selection()) {
    if(cheatText[position()][1] == " ") {
      cheatText[position()][1] = "X";
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
