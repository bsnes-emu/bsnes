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

void pListView::autoSizeColumns() {
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

void pListView::reset() {
  qtListView->clear();
}

bool pListView::selected() {
  QTreeWidgetItem *item = qtListView->currentItem();
  return (item && item->isSelected() == true);
}

unsigned pListView::selection() {
  QTreeWidgetItem *item = qtListView->currentItem();
  if(item == 0) return 0;
  return item->data(0, Qt::UserRole).toUInt();
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
  autoSizeColumns();
}

void pListView::setHeaderVisible(bool visible) {
  qtListView->setHeaderHidden(!visible);
  autoSizeColumns();
}

void pListView::setSelected(bool selected) {
  QTreeWidgetItem *item = qtListView->currentItem();
  if(item) item->setSelected(selected);
}

void pListView::setSelection(unsigned row) {
  locked = true;
  QTreeWidgetItem *item = qtListView->currentItem();
  if(item) item->setSelected(false);
  qtListView->setCurrentItem(0);
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
  qtListView->setAllColumnsShowFocus(true);
  qtListView->setRootIsDecorated(false);

  connect(qtListView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  connect(qtListView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(onChange(QTreeWidgetItem*)));
  connect(qtListView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onTick(QTreeWidgetItem*)));

  setCheckable(listView.state.checkable);
  setHeaderText(listView.state.headerText.size() ? listView.state.headerText : lstring{ " " });
  setHeaderVisible(listView.state.headerVisible);
  foreach(row, listView.state.text) append(row);
  if(listView.state.checkable) {
    for(unsigned n = 0; n < listView.state.checked.size(); n++) {
      setChecked(n, listView.state.checked[n]);
    }
  }
  setSelected(listView.state.selected);
  if(listView.state.selected) setSelection(listView.state.selection);
}

void pListView::destructor() {
  delete qtListView;
  qtWidget = qtListView = 0;
}

void pListView::orphan() {
  destructor();
  constructor();
}

void pListView::onActivate() {
  if(locked == false && listView.onActivate) listView.onActivate();
}

void pListView::onChange(QTreeWidgetItem *item) {
  //Qt bug workaround: clicking items with mouse does not mark items as selected
  if(item) item->setSelected(true);
  listView.state.selected = selected();
  if(listView.state.selected) listView.state.selection = selection();
  if(locked == false && listView.onChange) listView.onChange();
}

void pListView::onTick(QTreeWidgetItem *item) {
  unsigned row = item->data(0, Qt::UserRole).toUInt();
  bool checkState = checked(row);
  listView.state.checked[row] = checkState;
  if(locked == false && listView.onTick) listView.onTick(row);
}
