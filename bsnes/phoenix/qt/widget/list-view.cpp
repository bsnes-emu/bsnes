void pListView::append(const lstring &text) {
  locked = true;
  auto items = qtListView->findItems("", Qt::MatchContains);
  QTreeWidgetItem *item = new QTreeWidgetItem(qtListView);
  item->setData(0, Qt::UserRole, (unsigned)items.size());
  if(listView.state.checkable) item->setCheckState(0, Qt::Unchecked);
  for(unsigned n = 0; n < text.size(); n++) {
    item->setText(n, QString::fromUtf8(text[n]));
  }
  locked = false;
}

void pListView::autosizeColumns() {
  for(unsigned n = 0; n < listView.state.headerText.size(); n++) qtListView->resizeColumnToContents(n);
}

bool pListView::checked(unsigned row) {
  QTreeWidgetItem *item = qtListView->topLevelItem(row);
  return item ? item->checkState(0) == Qt::Checked : false;
}

void pListView::modify(unsigned row, const lstring &text) {
  locked = true;
  QTreeWidgetItem *item = qtListView->topLevelItem(row);
  if(!item) return;
  for(unsigned n = 0; n < text.size(); n++) {
    item->setText(n, QString::fromUtf8(text[n]));
  }
  locked = false;
}

void pListView::modify(unsigned row, unsigned column, const string &text) {
  locked = true;
  QTreeWidgetItem *item = qtListView->topLevelItem(row);
  if(!item) return;
  item->setText(column, QString::fromUtf8(text));
  locked = false;
}

void pListView::reset() {
  qtListView->clear();
}

optional<unsigned> pListView::selection() {
  QTreeWidgetItem *item = qtListView->currentItem();
  if(item == 0) return { false, 0 };
  if(item->isSelected() == false) return { false, 0 };
  return { true, item->data(0, Qt::UserRole).toUInt() };
}

void pListView::setCheckable(bool checkable) {
  if(checkable) {
    auto items = qtListView->findItems("", Qt::MatchContains);
    for(unsigned n = 0; n < items.size(); n++) items[n]->setCheckState(0, Qt::Unchecked);
  }
}

void pListView::setChecked(unsigned row, bool checked) {
  locked = true;
  QTreeWidgetItem *item = qtListView->topLevelItem(row);
  if(item) item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
  locked = false;
}

void pListView::setHeaderText(const lstring &text) {
  QStringList labels;
  foreach(column, text) labels << QString::fromUtf8(column);

  qtListView->setColumnCount(text.size());
  qtListView->setAlternatingRowColors(text.size() >= 2);
  qtListView->setHeaderLabels(labels);
  autosizeColumns();
}

void pListView::setHeaderVisible(bool visible) {
  qtListView->setHeaderHidden(!visible);
  autosizeColumns();
}

void pListView::setSelection(unsigned row) {
  locked = true;
  QTreeWidgetItem *item = qtListView->currentItem();
  if(item) item->setSelected(false);
  auto items = qtListView->findItems("", Qt::MatchContains);
  for(unsigned n = 0; n < items.size(); n++) {
    if(items[n]->data(0, Qt::UserRole).toUInt() == row) {
      qtListView->setCurrentItem(items[n]);
      break;
    }
  }
  locked = false;
}

void pListView::constructor() {
  qtWidget = qtListView = new QTreeWidget;
  qtListView->setHeaderLabels(QStringList() << "");
  qtListView->setHeaderHidden(true);
  qtListView->setAllColumnsShowFocus(true);
  qtListView->setRootIsDecorated(false);

  connect(qtListView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  connect(qtListView, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  connect(qtListView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onTick(QTreeWidgetItem*)));
}

void pListView::onActivate() {
  if(locked == false && listView.onActivate) listView.onActivate();
}

void pListView::onChange() {
  if(auto position = selection()) {
    listView.state.selection = { true, position() };
  } else {
    listView.state.selection = { false, 0 };
  }
  if(locked == false && listView.onChange) listView.onChange();
}

void pListView::onTick(QTreeWidgetItem *item) {
  unsigned row = item->data(0, Qt::UserRole).toUInt();
  bool checkState = checked(row);
  listView.state.checked[row] = checkState;
  if(locked == false && listView.onTick) listView.onTick(row);
}
