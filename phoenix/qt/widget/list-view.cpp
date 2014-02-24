namespace phoenix {

void pListView::append(const lstring& text) {
  locked = true;
  auto items = qtListView->findItems("", Qt::MatchContains);
  QTreeWidgetItem* item = new QTreeWidgetItem(qtListView);

  item->setData(0, Qt::UserRole, (unsigned)items.size());
  if(listView.state.checkable) item->setCheckState(0, Qt::Unchecked);
  for(unsigned position = 0; position < text.size(); position++) {
    item->setText(position, QString::fromUtf8(text[position]));
  }
  locked = false;
}

void pListView::autoSizeColumns() {
  for(unsigned n = 0; n < listView.state.headerText.size(); n++) qtListView->resizeColumnToContents(n);
}

void pListView::remove(unsigned selection) {
  locked = true;
  QTreeWidgetItem* item = qtListView->topLevelItem(selection);
  if(item == nullptr) return;
  delete item;
  locked = false;
}

void pListView::reset() {
  qtListView->clear();
}

void pListView::setBackgroundColor(Color color) {
}

void pListView::setCheckable(bool checkable) {
  if(checkable) {
    auto items = qtListView->findItems("", Qt::MatchContains);
    for(unsigned n = 0; n < items.size(); n++) items[n]->setCheckState(0, Qt::Unchecked);
  }
}

void pListView::setChecked(unsigned selection, bool checked) {
  locked = true;
  QTreeWidgetItem* item = qtListView->topLevelItem(selection);
  if(item) item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
  locked = false;
}

void pListView::setForegroundColor(Color color) {
}

void pListView::setHeaderText(const lstring& text) {
  QStringList labels;
  for(auto& column : text) labels << QString::fromUtf8(column);

  qtListView->setColumnCount(text.size());
  qtListView->setAlternatingRowColors(text.size() >= 2);
  qtListView->setHeaderLabels(labels);
  autoSizeColumns();
}

void pListView::setHeaderVisible(bool visible) {
  qtListView->setHeaderHidden(!visible);
  autoSizeColumns();
}

void pListView::setImage(unsigned selection, unsigned position, const nall::image& image) {
  QTreeWidgetItem* item = qtListView->topLevelItem(selection);
  if(item) {
    if(image.empty() == 0) item->setIcon(position, CreateIcon(image));
    if(image.empty() == 1) item->setIcon(position, QIcon());
  }
}

void pListView::setSelected(bool selected) {
  QTreeWidgetItem* item = qtListView->currentItem();
  if(item) item->setSelected(selected);
}

void pListView::setSelection(unsigned selection) {
  locked = true;
  QTreeWidgetItem* item = qtListView->currentItem();
  if(item) item->setSelected(false);
  item = qtListView->topLevelItem(selection);
  if(item) qtListView->setCurrentItem(item);
  locked = false;
}

void pListView::setText(unsigned selection, unsigned position, string text) {
  locked = true;
  QTreeWidgetItem* item = qtListView->topLevelItem(selection);
  if(item) item->setText(position, QString::fromUtf8(text));
  locked = false;
}

void pListView::constructor() {
  qtWidget = qtListView = new QTreeWidget;
  qtListView->setAllColumnsShowFocus(true);
  qtListView->setRootIsDecorated(false);

  connect(qtListView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  connect(qtListView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(onChange(QTreeWidgetItem*)));
  connect(qtListView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onToggle(QTreeWidgetItem*)));

  pWidget::synchronizeState();
  setCheckable(listView.state.checkable);
  setHeaderText(listView.state.headerText.size() ? listView.state.headerText : lstring{ " " });
  setHeaderVisible(listView.state.headerVisible);
  for(auto& row : listView.state.text) append(row);
  if(listView.state.checkable) {
    for(unsigned n = 0; n < listView.state.checked.size(); n++) {
      setChecked(n, listView.state.checked[n]);
    }
  }
  setSelected(listView.state.selected);
  if(listView.state.selected) setSelection(listView.state.selection);
  autoSizeColumns();
}

void pListView::destructor() {
  delete qtListView;
  qtWidget = qtListView = nullptr;
}

void pListView::orphan() {
  destructor();
  constructor();
}

void pListView::onActivate() {
  if(locked == false && listView.onActivate) listView.onActivate();
}

void pListView::onChange(QTreeWidgetItem* item) {
  bool selected = listView.state.selected;
  unsigned selection = listView.state.selection;
  if(item) {
    item->setSelected(true);  //Qt bug workaround: clicking items with mouse does not mark items as selected
    listView.state.selected = true;
    listView.state.selection = item->data(0, Qt::UserRole).toUInt();
  } else {
    listView.state.selected = false;
    listView.state.selection = 0;
  }
  if(selected != listView.state.selected || selection != listView.state.selection) {
    if(!locked && listView.onChange) listView.onChange();
  }
}

void pListView::onToggle(QTreeWidgetItem* item) {
  unsigned selection = item->data(0, Qt::UserRole).toUInt();
  listView.state.checked[selection] = (item->checkState(0) == Qt::Checked);
  if(!locked && listView.onToggle) listView.onToggle(selection);
}

}
