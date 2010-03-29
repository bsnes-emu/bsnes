#include "cheateditor.moc"
CheatEditorWindow *cheatEditorWindow;
CheatImportWindow *cheatImportWindow;

CheatEditorWindow::CheatEditorWindow() {
  lock = false;
  document = 0;

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  list = new QTreeWidget;
  list->setColumnCount(3);
  list->setHeaderLabels(QStringList() << "Slot" << "Code" << "Description");
  list->setColumnWidth(1, list->fontMetrics().width("  89AB-CDEF+...  "));
  list->setAllColumnsShowFocus(true);
  list->sortByColumn(0, Qt::AscendingOrder);
  list->setRootIsDecorated(false);
  list->setSelectionMode(QAbstractItemView::ExtendedSelection);
  list->resizeColumnToContents(0);
  layout->addWidget(list);

  gridLayout = new QGridLayout;
  layout->addLayout(gridLayout);

  codeLabel = new QLabel("Code(s):");
  codeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  gridLayout->addWidget(codeLabel, 0, 0);

  codeEdit = new QLineEdit;
  gridLayout->addWidget(codeEdit, 0, 1);

  descLabel = new QLabel("Description:");
  descLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  gridLayout->addWidget(descLabel, 1, 0);

  descEdit = new QLineEdit;
  gridLayout->addWidget(descEdit, 1, 1);

  controlLayout = new QHBoxLayout;
  layout->addLayout(controlLayout);

  cheatEnableBox = new QCheckBox("Enable Cheat Engine");
  cheatEnableBox->setToolTip("Unchecking this disables all cheat codes");
  cheatEnableBox->setChecked(SNES::cheat.enabled());
  controlLayout->addWidget(cheatEnableBox);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  controlLayout->addWidget(spacer);

  findButton = new QPushButton("Find Cheat Codes ...");
  controlLayout->addWidget(findButton);

  clearButton = new QPushButton("Clear Selected");
  controlLayout->addWidget(clearButton);

  cheatImportWindow = new CheatImportWindow;
  synchronize();

  connect(list, SIGNAL(itemSelectionChanged()), this, SLOT(listChanged()));
  connect(list, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(bind()));
  connect(codeEdit, SIGNAL(textEdited(const QString&)), this, SLOT(codeEdited()));
  connect(descEdit, SIGNAL(textEdited(const QString&)), this, SLOT(descEdited()));
  connect(cheatEnableBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCheatEnable()));
  connect(findButton, SIGNAL(released()), this, SLOT(findCheatCodes()));
  connect(clearButton, SIGNAL(released()), this, SLOT(clearSelected()));
}

void CheatEditorWindow::synchronize() {
  auto items = list->selectedItems();
  if(items.count() == 1) {
    descEdit->setEnabled(true);
    codeEdit->setEnabled(true);
  } else {
    descEdit->setText("");
    codeEdit->setText("");
    descEdit->setEnabled(false);
    codeEdit->setEnabled(false);
  }
  clearButton->setEnabled(items.count() > 0);
  findButton->setEnabled(SNES::cartridge.loaded());
  if(SNES::cartridge.loaded() == false && cheatImportWindow->isVisible()) cheatImportWindow->close();
}

void CheatEditorWindow::load(const char *filename) {
  lock = true;
  list->clear();
  list->setSortingEnabled(false);
  SNES::cheat.reset();

  string data;
  lstring line;

  if(data.readfile(filename)) {
    data.replace("\r", "");
    line.split("\n", data);
  }

  for(unsigned i = 0; i < 128; i++) {
    lstring part;
    if(line.size() > i) part.qsplit(",", line[i]);
    for(unsigned n = 0; n <= 2; n++) trim(part[n], " ");
    trim(part[0], "\"");
    trim(part[1], "\"");
    trim(part[2], "\"");
    part[2].replace("\\q", "\"");

    auto item = new QTreeWidgetItem(list);
    item->setData(0, Qt::UserRole, QVariant(i));
    item->setText(0, strunsigned<3, ' '>(i + 1));
    item->setCheckState(0, part[0] == "enabled" ? Qt::Checked : Qt::Unchecked);
    item->setText(1, part[1]);
    item->setText(2, part[2]);
  }

  list->resizeColumnToContents(0);
  list->setSortingEnabled(true);
  list->header()->setSortIndicatorShown(false);

  lock = false;
  bind();
  update();
}

void CheatEditorWindow::save(const char *filename) {
  bool empty = true;
  string data[128];

  auto items = list->findItems("", Qt::MatchContains);
  foreach(item, items) {
    unsigned index = item->data(0, Qt::UserRole).toUInt();
    string code = item->text(1).toUtf8().constData();
    string desc = item->text(2).toUtf8().constData();
    desc.replace("\"", "\\q");
    if((code != "") || (desc != "")) empty = false;

    data[index] << "\"" << (item->checkState(0) == Qt::Checked ? "enabled" : "disabled") << "\",";
    data[index] << "\"" << code << "\",";
    data[index] << "\"" << desc << "\"\r\n";
  }

  if(empty == true) {
    unlink(filename);
  } else {
    file fp;
    if(fp.open(filename, file::mode_write)) {
      //determine how many rows from the bottom up are empty, and exclude them from the file
      //eg if only the first three slots are used, don't save the last 125 empty slots
      unsigned last = 127;
      do {
        if(data[last] != "\"disabled\",\"\",\"\"\r\n") break;
      } while(--last);

      for(unsigned i = 0; i <= last; i++) fp.print(data[i]);
      fp.close();
    }
  }

  list->clear();
  SNES::cheat.reset();
  SNES::cheat.synchronize();
}

void CheatEditorWindow::update() {
  auto items = list->findItems("", Qt::MatchContains);
  foreach(item, items) {
    string code = item->text(1).toUtf8().constData();
    string desc = item->text(2).toUtf8().constData();
    if((code != "") || (desc != "")) {
      item->setForeground(0, QBrush(QColor(0, 0, 0)));
    } else {
      //highlight empty slots in gray
      item->setForeground(0, QBrush(QColor(128, 128, 128)));
    }
    unsigned index = item->data(0, Qt::UserRole).toUInt();
    if(SNES::cheat[index].addr.size() > 0) {
      item->setForeground(1, QBrush(QColor(0, 0, 0)));
    } else {
      //highlight invalid codes in red
      //(this will also highlight empty codes, but as there is no text, it's not an issue)
      item->setForeground(1, QBrush(QColor(255, 0, 0)));
    }
  }
}

void CheatEditorWindow::bind() {
  if(lock) return;

  auto items = list->findItems("", Qt::MatchContains);
  foreach(item, items) {
    unsigned index = item->data(0, Qt::UserRole).toUInt();
    SNES::cheat[index] = item->text(1).toUtf8().constData();
    SNES::cheat[index].enabled = item->checkState(0) == Qt::Checked;
  }
  SNES::cheat.synchronize();
}

void CheatEditorWindow::listChanged() {
  if(lock) return;

  auto items = list->selectedItems();
  if(items.count() > 0) {
    auto item = items[0];
    codeEdit->setText(item->text(1));
    descEdit->setText(item->text(2));
  }
  synchronize();
}

void CheatEditorWindow::codeEdited() {
  if(lock) return;

  auto items = list->selectedItems();
  if(items.count() == 1) {
    auto item = items[0];
    item->setText(1, codeEdit->text());
  }
  bind();
  update();
}

void CheatEditorWindow::descEdited() {
  if(lock) return;

  auto items = list->selectedItems();
  if(items.count() == 1) {
    auto item = items[0];
    item->setText(2, descEdit->text());
  }
  update();
}

void CheatEditorWindow::toggleCheatEnable() {
  SNES::cheat.enable(cheatEnableBox->isChecked());
}

void CheatEditorWindow::findCheatCodes() {
  string data;
  QFile file(":/cheats.xml");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    data = file.readAll().constData();
    file.close();
  }

  if(auto position = strpos(data, SNES::cartridge.sha256())) {
    const char *block = (const char*)data + position() - 19;
    if(position = strpos(block, "</cartridge>")) {
      xml_element document = xml_parse(substr(block, 0, position() + 12));
      if(document.element.size() == 0) return;

      cheatImportWindow->refresh(document.element[0]);
      cheatImportWindow->show();
      return;
    }
  }

  audio.clear();
  QMessageBox::information(toolsWindow, "Cheat Code Importer",
    "Sorry, no cheat codes were found for the currently loaded cartridge."
  );
}

void CheatEditorWindow::clearSelected() {
  if(lock) return;

  auto items = list->selectedItems();
  foreach(item, items) {
    item->setCheckState(0, Qt::Unchecked);
    item->setText(1, "");
    item->setText(2, "");
  }
  codeEdit->setText("");
  descEdit->setText("");
  bind();
  update();
}

//=================
//CheatImportWindow
//=================

void CheatImportWindow::refresh(xml_element &root) {
  list->clear();

  foreach(node, root.element) {
    if(node.name == "name") {
      title->setText(string() << "<b>Name:</b> " << node.parse());
    } else if(node.name == "cheat") {
      string description = "<undefined>";
      string code = "";

      foreach(leaf, node.element) {
        if(leaf.name == "description") {
          description = leaf.parse();
        } else if(leaf.name == "code") {
          if(code != "") code << "+";
          code << leaf.content;
        }
      }

      auto item = new QTreeWidgetItem(list);
      item->setCheckState(0, Qt::Unchecked);
      item->setText(0, string() << description);
      QString qcode = code;
      item->setData(0, Qt::UserRole, QVariant(qcode));
    }
  }
}

CheatImportWindow::CheatImportWindow() {
  resize(640, 360);

  setObjectName("cheat-import-window");
  setWindowTitle("Cheat Code Importer");
  setGeometryString(&config().geometry.cheatImportWindow);
  application.windowList.add(this);

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  title = new QLabel("Test");
  layout->addWidget(title);

  list = new QTreeWidget;
  list->setColumnCount(1);
  list->setAllColumnsShowFocus(true);
  list->setRootIsDecorated(false);
  list->setHeaderHidden(true);
  layout->addWidget(list);

  controlLayout = new QHBoxLayout;
  layout->addLayout(controlLayout);

  selectAllButton = new QPushButton("Select All");
  controlLayout->addWidget(selectAllButton);

  clearAllButton = new QPushButton("Clear All");
  controlLayout->addWidget(clearAllButton);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  controlLayout->addWidget(spacer);

  okButton = new QPushButton("Ok");
  controlLayout->addWidget(okButton);

  cancelButton = new QPushButton("Cancel");
  controlLayout->addWidget(cancelButton);

  connect(selectAllButton, SIGNAL(released()), this, SLOT(selectAllCodes()));
  connect(clearAllButton, SIGNAL(released()), this, SLOT(clearAllCodes()));
  connect(okButton, SIGNAL(released()), this, SLOT(addSelectedCodes()));
  connect(cancelButton, SIGNAL(released()), this, SLOT(close()));
}

void CheatImportWindow::selectAllCodes() {
  auto items = list->findItems("", Qt::MatchContains);
  foreach(item, items) item->setCheckState(0, Qt::Checked);
}

void CheatImportWindow::clearAllCodes() {
  auto items = list->findItems("", Qt::MatchContains);
  foreach(item, items) item->setCheckState(0, Qt::Unchecked);
}

void CheatImportWindow::addSelectedCodes() {
  auto items = list->findItems("", Qt::MatchContains);
  foreach(item, items) {
    if(item->checkState(0) == Qt::Checked) {
      auto codeslots = cheatEditorWindow->list->findItems("", Qt::MatchContains);
      foreach(codeslot, codeslots) {
        if(codeslot->text(1) == "" && codeslot->text(2) == "") {
          codeslot->setCheckState(0, Qt::Unchecked);
          codeslot->setText(1, item->data(0, Qt::UserRole).toString());
          codeslot->setText(2, item->text(0));
          break;
        }
      }
    }
  }

  cheatEditorWindow->bind();
  cheatEditorWindow->update();
  close();
}
