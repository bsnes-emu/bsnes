void pListBox::append(const lstring &text) {
  locked = true;
  auto items = qtListBox->findItems("", Qt::MatchContains);
  QTreeWidgetItem *item = new QTreeWidgetItem(qtListBox);
  item->setData(0, Qt::UserRole, (unsigned)items.size());
  if(listBox.state.checkable) item->setCheckState(0, Qt::Unchecked);
  for(unsigned n = 0; n < text.size(); n++) {
    item->setText(n, QString::fromUtf8(text[n]));
  }
  locked = false;
}

void pListBox::autosizeColumns() {
  for(unsigned n = 0; n < listBox.state.headerText.size(); n++) qtListBox->resizeColumnToContents(n);
}

bool pListBox::checked(unsigned row) {
  QTreeWidgetItem *item = qtListBox->topLevelItem(row);
  return item ? item->checkState(0) == Qt::Checked : false;
}

void pListBox::modify(unsigned row, const lstring &text) {
  QTreeWidgetItem *item = qtListBox->topLevelItem(row);
  if(!item) return;
  for(unsigned n = 0; n < text.size(); n++) {
    item->setText(n, QString::fromUtf8(text[n]));
  }
}

void pListBox::modify(unsigned row, unsigned column, const string &text) {
  QTreeWidgetItem *item = qtListBox->topLevelItem(row);
  if(!item) return;
  item->setText(column, QString::fromUtf8(text));
}

void pListBox::reset() {
  qtListBox->clear();
}

optional<unsigned> pListBox::selection() {
  QTreeWidgetItem *item = qtListBox->currentItem();
  if(item == 0) return { false, 0 };
  if(item->isSelected() == false) return { false, 0 };
  return { true, item->data(0, Qt::UserRole).toUInt() };
}

void pListBox::setCheckable(bool checkable) {
  if(checkable) {
    auto items = qtListBox->findItems("", Qt::MatchContains);
    for(unsigned n = 0; n < items.size(); n++) items[n]->setCheckState(0, Qt::Unchecked);
  }
}

void pListBox::setChecked(unsigned row, bool checked) {
  locked = true;
  QTreeWidgetItem *item = qtListBox->topLevelItem(row);
  if(item) item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
  locked = false;
}

void pListBox::setHeaderText(const lstring &text) {
  QStringList labels;
  foreach(column, text) labels << QString::fromUtf8(column);

  qtListBox->setColumnCount(text.size());
  qtListBox->setAlternatingRowColors(text.size() >= 2);
  qtListBox->setHeaderLabels(labels);
  autosizeColumns();
}

void pListBox::setHeaderVisible(bool visible) {
  qtListBox->setHeaderHidden(!visible);
  autosizeColumns();
}

void pListBox::setSelection(unsigned row) {
  locked = true;
  QTreeWidgetItem *item = qtListBox->currentItem();
  if(item) item->setSelected(false);
  auto items = qtListBox->findItems("", Qt::MatchContains);
  for(unsigned n = 0; n < items.size(); n++) {
    if(items[n]->data(0, Qt::UserRole).toUInt() == row) {
      qtListBox->setCurrentItem(items[n]);
      break;
    }
  }
  locked = false;
}

pListBox::pListBox(ListBox &listBox) : listBox(listBox), pWidget(listBox) {
  qtWidget = qtListBox = new QTreeWidget;
  qtListBox->setHeaderLabels(QStringList() << "");
  qtListBox->setHeaderHidden(true);
  qtListBox->setAllColumnsShowFocus(true);
  qtListBox->setRootIsDecorated(false);

  connect(qtListBox, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  connect(qtListBox, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  connect(qtListBox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onTick(QTreeWidgetItem*)));
}

void pListBox::onActivate() {
  if(locked == false && listBox.onActivate) listBox.onActivate();
}

void pListBox::onChange() {
  if(auto position = selection()) {
    listBox.state.selection = { true, position() };
  } else {
    listBox.state.selection = { false, 0 };
  }
  if(locked == false && listBox.onChange) listBox.onChange();
}

void pListBox::onTick(QTreeWidgetItem *item) {
  unsigned row = item->data(0, Qt::UserRole).toUInt();
  bool checkState = checked(row);
  listBox.state.checked[row] = checkState;
  if(locked == false && listBox.onTick) listBox.onTick(row);
}
