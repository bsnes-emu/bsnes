void ListBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  listBox->setParent(parent.window->container);
  listBox->setGeometry(x, y, width, height);
  listBox->setAllColumnsShowFocus(true);
  listBox->setRootIsDecorated(false);

  lstring list;
  list.split("\t", text);
  QStringList labels;
  foreach(item, list) labels << (const char*)item;
  listBox->setColumnCount(list.size());
  listBox->setHeaderLabels(labels);
  for(unsigned i = 0; i < list.size(); i++) listBox->resizeColumnToContents(i);

  listBox->setHeaderHidden(true);
  listBox->setAlternatingRowColors(list.size() >= 2);
  listBox->connect(listBox, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  listBox->connect(listBox, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  if(parent.window->defaultFont) listBox->setFont(*parent.window->defaultFont);
  listBox->show();
}

void ListBox::setHeaderVisible(bool headerVisible) {
  listBox->setHeaderHidden(headerVisible == false);
}

void ListBox::reset() {
  listBox->clear();
}

void ListBox::resizeColumnsToContent() {
  for(unsigned i = 0; i < listBox->columnCount(); i++) listBox->resizeColumnToContents(i);
}

void ListBox::addItem(const char *text) {
  auto items = listBox->findItems("", Qt::MatchContains);
  QTreeWidgetItem *item = new QTreeWidgetItem(listBox);
  item->setData(0, Qt::UserRole, (unsigned)items.size());
  lstring list;
  list.split("\t", text);
  for(unsigned i = 0; i < list.size(); i++) item->setText(i, (const char*)list[i]);
}

void ListBox::setItem(unsigned row, const char *text) {
  QTreeWidgetItem *item = listBox->topLevelItem(row);
  lstring list;
  list.split("\t", text);
  for(unsigned i = 0; i < list.size(); i++) item->setText(i, (const char*)list[i]);
}

optional<unsigned> ListBox::selection() {
  QTreeWidgetItem *item = listBox->currentItem();
  if(item == 0) return { false, 0 };
  if(item->isSelected() == false) return { false, 0 };
  unsigned row = item->data(0, Qt::UserRole).toUInt();
  return { true, row };
}

void ListBox::setSelection(unsigned row) {
  object->locked = true;
  QTreeWidgetItem *item = listBox->currentItem();
  if(item) item->setSelected(false);
  auto items = listBox->findItems("", Qt::MatchContains);
  for(unsigned i = 0; i < items.size(); i++) {
    if(items[i]->data(0, Qt::UserRole).toUInt() == row) {
      listBox->setCurrentItem(items[i]);
      break;
    }
  }
  object->locked = false;
}

ListBox::ListBox() {
  listBox = new ListBox::Data(*this);
  widget->widget = listBox;
}
