void CodeEditorWindow::setup() {
  window = new QWidget;
  window->setObjectName("code-editor-window");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0);

  descLabel = new QLabel("Description:");
  layout->addWidget(descLabel);

  description = new QTextEdit;
  layout->addWidget(description);
  layout->addSpacing(Style::WidgetSpacing);

  codeLabel = new QLabel("Cheat code(s):");
  layout->addWidget(codeLabel);

  codeLayout = new QHBoxLayout; {
    codeLayout->setMargin(0);

    codeList = new QListWidget;
    codeLayout->addWidget(codeList);
    codeLayout->addSpacing(Style::WidgetSpacing);

    controls = new QVBoxLayout; {
      controls->setMargin(0);

      codeValue = new QLineEdit;
      controls->addWidget(codeValue);

      codeAdd = new QPushButton("Add Code");
      controls->addWidget(codeAdd);

      codeDelete = new QPushButton("Delete Code");
      controls->addWidget(codeDelete);

      codeDeleteAll = new QPushButton("Delete All");
      controls->addWidget(codeDeleteAll);

      spacer = new QWidget;
      spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
      controls->addWidget(spacer);
    }
    codeLayout->addLayout(controls);
  }
  layout->addLayout(codeLayout);
  layout->addSpacing(Style::WidgetSpacing);

  enabled = new QCheckBox("Enable this cheat code");
  layout->addWidget(enabled);

  finishControls = new QHBoxLayout; {
    okButton = new QPushButton("Ok");
    finishControls->addWidget(okButton);

    cancelButton = new QPushButton("Cancel");
    finishControls->addWidget(cancelButton);
  }
  finishControls->setSpacing(Style::WidgetSpacing);
  layout->addLayout(finishControls);

  window->setLayout(layout);
  window->setMinimumSize(400, 375);

  connect(codeList, SIGNAL(itemSelectionChanged()), this, SLOT(listChanged()));
  connect(codeValue, SIGNAL(textChanged(const QString&)), this, SLOT(codeChanged()));
  connect(codeAdd, SIGNAL(released()), this, SLOT(addCodeToList()));
  connect(codeDelete, SIGNAL(released()), this, SLOT(deleteCodeFromList()));
  connect(codeDeleteAll, SIGNAL(released()), this, SLOT(deleteAllCodesFromList()));
  connect(okButton, SIGNAL(released()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(released()), this, SLOT(dismiss()));
}

void CodeEditorWindow::syncUi() {
  //only activate add button when code is valid
  string code = codeValue->text().toUtf8().data();
  Cheat::cheat_t temp;
  bool valid = cheat.decode(code, temp);
  codeAdd->setEnabled(valid);

  //only activate delete button when a code is selected
  QListWidgetItem *item = codeList->currentItem();
  codeDelete->setEnabled(item && item->isSelected());

  //only activate delete all / ok buttons when there are one or more codes entered
  codeDeleteAll->setEnabled(codeList->count() > 0);
  okButton->setEnabled(codeList->count() > 0);
}

void CodeEditorWindow::listChanged() { syncUi(); }
void CodeEditorWindow::codeChanged() { syncUi(); }

void CodeEditorWindow::addCodeToList() {
  string code = codeValue->text().toUtf8().data();
  Cheat::cheat_t temp;
  if(cheat.decode(code, temp) == true) codeList->addItem(utf8() << code);
  syncUi();
}

void CodeEditorWindow::deleteCodeFromList() {
  int index = codeList->currentRow();
  if(index >= 0) {
    QListWidgetItem *item = codeList->takeItem(index);
    delete item;
  }
  syncUi();
}

void CodeEditorWindow::deleteAllCodesFromList() {
  codeList->clear();
  syncUi();
}

void CodeEditorWindow::accept() {
  string desc = description->toPlainText().toUtf8().data();
  string code;
  for(unsigned i = 0; i < codeList->count(); i++) {
    code << (codeList->item(i)->text().toUtf8().data());
    if(i != codeList->count() - 1) code << "+";
  }

  if(activeCode == -1) {
    //adding a new code
    cheat.add(enabled->isChecked(), code, desc);
    winCheatEditor->reloadList();
  } else if(codeList->count() > 0) {
    //editing an existing code
    cheat.edit(activeCode, enabled->isChecked(), code, desc);
    winCheatEditor->updateList();
  } else {
    //deleting an existing code
    cheat.remove(activeCode);
    winCheatEditor->reloadList();
  }

  dismiss();
}

void CodeEditorWindow::dismiss() {
  activeCode = -1;
  window->hide();
}

void CodeEditorWindow::addCode() {
  activeCode = -1;
  description->setPlainText("");
  codeList->clear();
  codeValue->setText("");
  enabled->setCheckState(Qt::Unchecked);
  showWindow("Add New Cheat Code");
}

void CodeEditorWindow::editCode(unsigned code) {
  activeCode = code;
  codeList->clear();
  codeValue->setText("");

  Cheat::cheat_t item;
  cheat.get(activeCode, item);

  description->setPlainText(utf8() << item.desc);

  lstring part;
  part.split("+", item.code);

  for(unsigned i = 0; i < item.count; i++) codeList->addItem(utf8() << part[i]);
  enabled->setCheckState(item.enabled ? Qt::Checked : Qt::Unchecked);
  showWindow("Edit Existing Cheat Code");
}

void CodeEditorWindow::showWindow(const char *title) {
  syncUi();
  window->setWindowTitle(title);
  utility.showCentered(window);
}

CodeEditorWindow::CodeEditorWindow() {
  activeCode = -1;
}
