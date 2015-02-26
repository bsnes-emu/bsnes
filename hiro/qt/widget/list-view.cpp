namespace phoenix {

void pListView::appendColumn() {
  orphan();
}

void pListView::appendItem() {
  lock();
  auto item = new QTreeWidgetItem(qtListView);
  for(unsigned column = 0; column < listView.columns(); column++) {
    auto& state = listView.state.columns[column];
    if(state.backgroundColor) {
      item->setBackground(column, QColor(
        state.backgroundColor->red, state.backgroundColor->green, state.backgroundColor->blue
      ));
    }
    if(state.font) {
      item->setFont(column, pFont::create(*state.font));
    }
    if(state.foregroundColor) {
      item->setForeground(column, QColor(
        state.foregroundColor->red, state.foregroundColor->green, state.foregroundColor->blue
      ));
    }
    item->setTextAlignment(column, calculateAlignment(
      state.horizontalAlignment, state.verticalAlignment
    ));
  }
  if(listView.state.checkable) item->setCheckState(0, Qt::Unchecked);
  unlock();
}

void pListView::removeColumn(unsigned position) {
  orphan();
}

void pListView::removeItem(unsigned position) {
  lock();
  if(auto item = qtListView->topLevelItem(position)) {
    delete item;
  }
  unlock();
}

void pListView::reset() {
  lock();
  qtListView->clear();
  unlock();
}

void pListView::resizeColumns() {
}

//todo: this doesn't work ...
void pListView::setActiveColumn(unsigned column) {
  if(column >= listView.columns()) return;
  qtListView->header()->setSortIndicator(column, Qt::DescendingOrder);
}

void pListView::setBackgroundColor(Color color) {
  QPalette palette = qtListView->palette();
  palette.setColor(QPalette::Base, QColor(color.red, color.green, color.blue));
  palette.setColor(QPalette::AlternateBase, QColor(max(0, (signed)color.red - 17), max(0, (signed)color.green - 17), max(0, (signed)color.blue - 17)));
  qtListView->setPalette(palette);
  qtListView->setAutoFillBackground(true);
}

void pListView::setCheckable(bool checkable) {
  lock();
  if(checkable) {
    for(unsigned n = 0; n < qtListView->topLevelItemCount(); n++) {
      if(auto item = qtListView->topLevelItem(n)) {
        item->setCheckState(0, Qt::Unchecked);
      }
    }
  }
  unlock();
}

void pListView::setChecked(unsigned position, bool checked) {
  lock();
  if(auto item = qtListView->topLevelItem(position)) {
    item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
  }
  unlock();
}

void pListView::setChecked(const vector<unsigned>& selections) {
  lock();
  setCheckedNone();
  for(auto& position : selections) setChecked(position, true);
  unlock();
}

void pListView::setCheckedAll() {
  lock();
  for(unsigned n = 0; n < qtListView->topLevelItemCount(); n++) {
    if(auto item = qtListView->topLevelItem(n)) {
      item->setCheckState(0, Qt::Checked);
    }
  }
  unlock();
}

void pListView::setCheckedNone() {
  lock();
  for(unsigned n = 0; n < qtListView->topLevelItemCount(); n++) {
    if(auto item = qtListView->topLevelItem(n)) {
      item->setCheckState(0, Qt::Unchecked);
    }
  }
  unlock();
}

void pListView::setColumnBackgroundColor(unsigned column, maybe<Color> color) {
  for(unsigned row = 0; row < listView.items(); row++) {
    if(auto item = qtListView->topLevelItem(row)) {
      item->setBackground(column, color ? QColor(color->red, color->green, color->blue) : QBrush());
    }
  }
}

void pListView::setColumnEditable(unsigned column, bool editable) {
}

void pListView::setColumnFont(unsigned column, maybe<string> font) {
  auto qtFont = pFont::create(font ? *font : widget.state.font);
  for(unsigned row = 0; row < listView.items(); row++) {
    if(auto item = qtListView->topLevelItem(row)) {
      item->setFont(column, qtFont);
    }
  }
}

void pListView::setColumnForegroundColor(unsigned column, maybe<Color> color) {
  for(unsigned row = 0; row < listView.items(); row++) {
    if(auto item = qtListView->topLevelItem(row)) {
      item->setForeground(column, color ? QColor(color->red, color->green, color->blue) : QBrush());
    }
  }
}

void pListView::setColumnHorizontalAlignment(unsigned column, double alignment) {
  qtListView->headerItem()->setTextAlignment(column, calculateAlignment(alignment, 0.5));
  for(unsigned row = 0; row < listView.items(); row++) {
    if(auto item = qtListView->topLevelItem(row)) {
      auto& state = listView.state.columns[column];
      item->setTextAlignment(column, calculateAlignment(state.horizontalAlignment, state.verticalAlignment));
    }
  }
}

void pListView::setColumnResizable(unsigned column, bool resizable) {
  qtListView->header()->setResizeMode(column, resizable ? QHeaderView::Interactive : QHeaderView::Fixed);
}

void pListView::setColumnSortable(unsigned column, bool sortable) {
  bool clickable = false;
  for(auto& column : listView.state.columns) clickable |= column.sortable;
  qtListView->header()->setClickable(clickable);
}

void pListView::setColumnText(unsigned column, const string& text) {
  qtListView->headerItem()->setText(column, QString::fromUtf8(text));
}

void pListView::setColumnVerticalAlignment(unsigned column, double alignment) {
  for(unsigned row = 0; row < listView.items(); row++) {
    if(auto item = qtListView->topLevelItem(row)) {
      auto& state = listView.state.columns[column];
      item->setTextAlignment(column, calculateAlignment(state.horizontalAlignment, state.verticalAlignment));
    }
  }
}

void pListView::setColumnVisible(unsigned column, bool visible) {
  if(column >= listView.columns()) return;
  qtListView->setColumnHidden(column, !visible);
}

void pListView::setColumnWidth(unsigned column, signed width) {
  if(column >= listView.columns()) return;
  resizeColumns();
}

void pListView::setForegroundColor(Color color) {
  QPalette palette = qtListView->palette();
  palette.setColor(QPalette::Text, QColor(color.red, color.green, color.blue));
  qtListView->setPalette(palette);
}

void pListView::setGridVisible(bool visible) {
  qtListView->repaint();
}

void pListView::setHeaderVisible(bool visible) {
  qtListView->setHeaderHidden(!visible);
  resizeColumns();
}

void pListView::setImage(unsigned row, unsigned column, const nall::image& image) {
  if(auto item = qtListView->topLevelItem(row)) {
    item->setIcon(column, CreateIcon(image));
  }
}

void pListView::setSelected(unsigned position, bool selected) {
  lock();
  if(auto item = qtListView->topLevelItem(position)) {
    item->setSelected(selected);
  }
  unlock();
}

void pListView::setSelected(const vector<unsigned>& selections) {
  lock();
  setSelectedNone();
  if(selections.size()) {
    if(auto item = qtListView->topLevelItem(selections[0])) {
      qtListView->setCurrentItem(item);
    }
    for(auto& position : selections) setSelected(position, true);
  }
  unlock();
}

void pListView::setSelectedAll() {
  lock();
  qtListView->selectAll();
  unlock();
}

void pListView::setSelectedNone() {
  lock();
  qtListView->clearSelection();
  unlock();
}

void pListView::setSingleSelection(bool singleSelection) {
  lock();
  qtListView->setSelectionMode(singleSelection ? QAbstractItemView::SingleSelection : QAbstractItemView::ExtendedSelection);
  unlock();
}

void pListView::setText(unsigned row, unsigned column, string text) {
  lock();
  if(auto item = qtListView->topLevelItem(row)) {
    item->setText(column, QString::fromUtf8(text));
  }
  unlock();
}

void pListView::constructor() {
  qtWidget = qtListView = new QtTreeWidget(*this);
  qtListView->setAllColumnsShowFocus(true);
  qtListView->setAlternatingRowColors(listView.columns() >= 2);
  qtListView->setColumnCount(max(1u, listView.columns()));
  qtListView->setContextMenuPolicy(Qt::CustomContextMenu);
  qtListView->setRootIsDecorated(false);
  qtListView->header()->setMovable(false);

  qtListViewDelegate = new QtTreeWidgetDelegate(*this);
  qtListView->setItemDelegate(qtListViewDelegate);

  connect(qtListView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  connect(qtListView, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  connect(qtListView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onContext()));
  connect(qtListView->header(), SIGNAL(sectionClicked(int)), SLOT(onSort(int)));
  connect(qtListView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onToggle(QTreeWidgetItem*)));

  pWidget::synchronizeState();
  for(unsigned column = 0; column < listView.columns(); column++) {
    auto& state = listView.state.columns[column];
    setColumnBackgroundColor(column, state.backgroundColor);
    setColumnEditable(column, state.editable);
    setColumnFont(column, state.font);
    setColumnForegroundColor(column, state.foregroundColor);
  //setColumnHorizontalAlignment(column, state.horizontalAlignment);
    setColumnResizable(column, state.resizable);
    setColumnSortable(column, state.sortable);
    setColumnText(column, state.text);
  //setColumnVerticalAlignment(column, state.verticalAlignment);
    setColumnVisible(column, state.visible);
  //setColumnWidth(column, state.width);
    qtListView->headerItem()->setTextAlignment(column, calculateAlignment(state.horizontalAlignment, 0.5));
  }
  setActiveColumn(listView.state.activeColumn);
  setCheckable(listView.state.checkable);
//setGridVisible(listView.state.gridVisible);
  setHeaderVisible(listView.state.headerVisible);
  setSingleSelection(listView.state.singleSelection);
  for(unsigned row = 0; row < listView.items(); row++) {
    appendItem();
    setSelected(row, listView.state.items[row].selected);
    if(listView.state.checkable) {
      setChecked(row, listView.state.items[row].checked);
    }
    for(unsigned column = 0; column < listView.columns(); column++) {
      setImage(row, column, listView.state.items[row].image(column, {}));
      setText(row, column, listView.state.items[row].text(column, ""));
    }
  }
  resizeColumns();
}

void pListView::destructor() {
  delete qtListViewDelegate;
  delete qtListView;
  qtWidget = qtListView = nullptr;
  qtListViewDelegate = nullptr;
}

void pListView::orphan() {
  destructor();
  constructor();
}

void pListView::onActivate() {
  if(!locked() && listView.onActivate) listView.onActivate();
}

void pListView::onChange() {
  for(auto& item : listView.state.items) item.selected = false;
  for(unsigned position = 0; position < qtListView->topLevelItemCount(); position++) {
    if(auto item = qtListView->topLevelItem(position)) {
      if(item->isSelected()) listView.state.items[position].selected = true;
    }
  }
  if(!locked() && listView.onChange) listView.onChange();
}

void pListView::onContext() {
  if(!locked() && listView.onContext) listView.onContext();
}

void pListView::onSort(int column) {
  if(column >= listView.columns()) return;
  if(listView.state.columns[column].sortable) {
    if(!locked() && listView.onSort) listView.onSort(column);
  }
}

void pListView::onToggle(QTreeWidgetItem* item) {
  maybe<unsigned> row;
  for(unsigned position = 0; position < qtListView->topLevelItemCount(); position++) {
    if(auto topLevelItem = qtListView->topLevelItem(position)) {
      if(topLevelItem == item) {
        row = position;
        break;
      }
    }
  }
  if(row) {
    listView.state.items[*row].checked = (item->checkState(0) == Qt::Checked);
    if(!locked() && listView.onToggle) listView.onToggle(*row);
  }
}

int pListView::calculateAlignment(double horizontal, double vertical) {
  int alignment = 0;
  if(horizontal < 0.333) alignment |= Qt::AlignLeft;
  else if(horizontal > 0.666) alignment |= Qt::AlignRight;
  else alignment |= Qt::AlignCenter;
  if(vertical < 0.333) alignment |= Qt::AlignTop;
  else if(vertical > 0.666) alignment |= Qt::AlignBottom;
  else alignment |= Qt::AlignVCenter;
  return alignment;
}

void pListView::QtTreeWidget::mousePressEvent(QMouseEvent* event) {
  QTreeWidget::mousePressEvent(event);

  if(event->button() == Qt::RightButton) {
    self.onContext();
  }
}

pListView::QtTreeWidget::QtTreeWidget(pListView& self) : self(self) {
}

void pListView::QtTreeWidgetDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
  QStyledItemDelegate::paint(painter, option, index);
  if(self.listView.state.gridVisible) {
    QPen pen;
    pen.setColor(QColor(192, 192, 192));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(option.rect);
  }
}

pListView::QtTreeWidgetDelegate::QtTreeWidgetDelegate(pListView& self) : QStyledItemDelegate(self.qtListView), self(self) {
}

}
