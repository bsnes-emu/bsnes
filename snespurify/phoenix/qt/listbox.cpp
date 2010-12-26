void ListBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  listBox->setParent(parent.window->container);
  listBox->setGeometry(x, y, width, height);
  listBox->setAllColumnsShowFocus(true);
  listBox->setRootIsDecorated(false);

  lstring list;
  list.split("\t", text);
  QStringList labels;
  foreach(item, list) labels << QString::fromUtf8(item);
  listBox->setColumnCount(list.size());
  listBox->setHeaderLabels(labels);
  for(unsigned i = 0; i < list.size(); i++) listBox->resizeColumnToContents(i);

  listBox->setHeaderHidden(true);
  listBox->setAlternatingRowColors(list.size() >= 2);
  listBox->connect(listBox, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  listBox->connect(listBox, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  listBox->connect(listBox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onTick(QTreeWidgetItem*)));
  if(parent.window->defaultFont) listBox->setFont(*parent.window->defaultFont);
  listBox->show();
}

void ListBox::setHeaderVisible(bool headerVisible) {
  listBox->setHeaderHidden(headerVisible == false);
}

void ListBox::setCheckable(bool checkable) {
  listBox->checkable = checkable;
  if(listBox->checkable) {
    auto items = listBox->findItems("", Qt::MatchContains);
    for(unsigned i = 0; i < items.size(); i++) items[i]->setCheckState(0, Qt::Unchecked);
  }
}

void ListBox::reset() {
  listBox->clear();
}

void ListBox::resizeColumnsToContent() {
  for(unsigned i = 0; i < listBox->columnCount(); i++) listBox->resizeColumnToContents(i);
}

void ListBox::addItem(const string &text) {
  object->locked = true;
  auto items = listBox->findItems("", Qt::MatchContains);
  QTreeWidgetItem *item = new QTreeWidgetItem(listBox);
  if(listBox->checkable) item->setCheckState(0, Qt::Unchecked);
  item->setData(0, Qt::UserRole, (unsigned)items.size());
  lstring list;
  list.split("\t", text);
  for(unsigned i = 0; i < list.size(); i++) item->setText(i, QString::fromUtf8(list[i]));
  object->locked = false;
}

void ListBox::setItem(unsigned row, const string &text) {
  object->locked = true;
  QTreeWidgetItem *item = listBox->topLevelItem(row);
  lstring list;
  list.split("\t", text);
  for(unsigned i = 0; i < list.size(); i++) item->setText(i, QString::fromUtf8(list[i]));
  object->locked = false;
}

bool ListBox::checked(unsigned row) {
  QTreeWidgetItem *item = listBox->topLevelItem(row);
  return (item ? item->checkState(0) == Qt::Checked : false);
}

void ListBox::setChecked(unsigned row, bool checked) {
  object->locked = true;
  QTreeWidgetItem *item = listBox->topLevelItem(row);
  if(item) item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
  object->locked = false;
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
