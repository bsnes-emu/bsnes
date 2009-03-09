void CheatEditorWindow::setup() {
  panel = new QWidget;

  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  title = new QLabel("Cheat Code Editor");
  title->setProperty("class", "title");
  layout->addWidget(title);

  list = new QTreeWidget;
  list->setColumnCount(4);
  list->setHeaderLabels(QStringList() << "Hidden" << "" << "Code" << "Description");
  list->sortByColumn(0, Qt::AscendingOrder);  //set initial sorting, preserve user setting after reloadList()
  list->hideColumn(0);  //used for default sorting + hides child expansion box
  layout->addWidget(list);
  layout->addSpacing(Style::WidgetSpacing);

  controls = new QHBoxLayout; {
    addCode = new QPushButton("Add Code ...");
    controls->addWidget(addCode);

    editCode = new QPushButton("Edit Code ...");
    controls->addWidget(editCode);

    deleteCode = new QPushButton("Delete Code");
    controls->addWidget(deleteCode);
  }
  controls->setSpacing(Style::WidgetSpacing);
  layout->addLayout(controls);

  panel->setLayout(layout);
  connect(list, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*)));
  connect(list, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(editSelectedCode()));
  connect(list, SIGNAL(itemSelectionChanged()), this, SLOT(listChanged()));
  connect(addCode, SIGNAL(released()), this, SLOT(addNewCode()));
  connect(editCode, SIGNAL(released()), this, SLOT(editSelectedCode()));
  connect(deleteCode, SIGNAL(released()), this, SLOT(deleteSelectedCode()));

  reloadList();
}

void CheatEditorWindow::syncUi() {
  addCode->setEnabled(cartridge.loaded());
  QList<QTreeWidgetItem*> itemList = list->selectedItems();
  editCode->setEnabled(cartridge.loaded() && itemList.count() == 1);
  deleteCode->setEnabled(cartridge.loaded() && itemList.count() == 1);
}

//called when loading a new game, or after adding / deleting a code:
//synchronizes list with cheat class list
void CheatEditorWindow::reloadList() {
  disconnect(list, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*)));

  list->clear();
  list->setSortingEnabled(false);
  listItem.reset();

  if(cartridge.loaded()) {
    for(unsigned i = 0; i < cheat.count(); i++) {
      Cheat::cheat_t code;
      cheat.get(i, code);

      //only want to show one code / description line in list
      lstring lcode, ldesc;
      lcode.split("+",  code.code);
      ldesc.split("\n", code.desc);
      if(lcode.size() > 1) lcode[0] << "+" << (int)(lcode.size() - 1);
      if(ldesc.size() > 1) ldesc[0] << " ...";

      QTreeWidgetItem *item = new QTreeWidgetItem(list);
      item->setText(0, utf8() << (int)(1000000 + i));
      item->setCheckState(1, code.enabled ? Qt::Checked : Qt::Unchecked);
      item->setText(2, utf8() << lcode[0]);
      item->setText(3, utf8() << ldesc[0]);
      listItem.add(item);
    }
  }

  list->setSortingEnabled(true);
  list->resizeColumnToContents(1);  //shrink checkbox column width
  list->resizeColumnToContents(2);  //shrink code column width
  connect(list, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*)));
  syncUi();
}

//called when modifying an existing code:
//list items are all still valid, only need to update item codes + descriptions
void CheatEditorWindow::updateList() {
  disconnect(list, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*)));

  for(unsigned i = 0; i < listItem.size(); i++) {
    Cheat::cheat_t code;
    cheat.get(i, code);

    //only want to show one code / description line in list
    lstring lcode, ldesc;
    lcode.split("+",  code.code);
    ldesc.split("\n", code.desc);
    if(lcode.size() > 1) lcode[0] << "+" << (int)(lcode.size() - 1);
    if(ldesc.size() > 1) ldesc[0] << " ...";

    QTreeWidgetItem *item = listItem[i];
    item->setCheckState(1, code.enabled ? Qt::Checked : Qt::Unchecked);
    item->setText(2, utf8() << lcode[0]);
    item->setText(3, utf8() << ldesc[0]);
  }

  list->resizeColumnToContents(1);  //shrink checkbox column width
  list->resizeColumnToContents(2);  //shrink code column width
  connect(list, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*)));
  syncUi();
}

//called when item enabled checkbox was clicked (eg cheat code enable state was toggled on or off)
void CheatEditorWindow::itemChanged(QTreeWidgetItem *item) {
  signed i = listItem.find(item);
  if(i >= 0) item->checkState(1) == Qt::Checked ? cheat.enable(i) : cheat.disable(i);
}

void CheatEditorWindow::listChanged() {
  syncUi();
}

void CheatEditorWindow::addNewCode() {
  if(cartridge.loaded()) winCodeEditor->addCode();
}

void CheatEditorWindow::editSelectedCode() {
  if(cartridge.loaded()) {
    QTreeWidgetItem *item = list->currentItem();
    if(item && item->isSelected()) {
      signed i = listItem.find(item);
      if(i >= 0) winCodeEditor->editCode(i);
    }
  }
}

void CheatEditorWindow::deleteSelectedCode() {
  if(cartridge.loaded()) {
    QTreeWidgetItem *item = list->currentItem();
    if(item && item->isSelected()) {
      signed i = listItem.find(item);
      if(i >= 0) {
        //if code editor is modifying an existing code, its index will be invalidated by delete;
        //therefore, the editor window must be closed first
        if(winCodeEditor->activeCode >= 0) winCodeEditor->dismiss();

        //remove code, and resync listItem with cheat list
        cheat.remove(i);
        reloadList();
      }
    }
  }
}
