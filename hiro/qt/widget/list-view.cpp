#if defined(Hiro_ListView)

namespace hiro {

auto pListView::construct() -> void {
  qtWidget = qtListView = new QtListView(*this);
  qtListView->setAllColumnsShowFocus(true);
  qtListView->setContextMenuPolicy(Qt::CustomContextMenu);
  qtListView->setRootIsDecorated(false);
  qtListView->setHeaderHidden(true);
  qtListView->header()->setMovable(false);

  qtListViewDelegate = new QtListViewDelegate(*this);
  qtListView->setItemDelegate(qtListViewDelegate);

  qtListView->connect(qtListView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  qtListView->connect(qtListView, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  qtListView->connect(qtListView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onContext()));
  qtListView->connect(qtListView->header(), SIGNAL(sectionClicked(int)), SLOT(onSort(int)));
  qtListView->connect(qtListView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onToggle(QTreeWidgetItem*, int)));

  setBackgroundColor(state().backgroundColor);
  setBatchable(state().batchable);
  setBordered(state().bordered);
  setForegroundColor(state().foregroundColor);

  pWidget::construct();
}

auto pListView::destruct() -> void {
  delete qtListViewDelegate;
  delete qtListView;
  qtWidget = qtListView = nullptr;
  qtListViewDelegate = nullptr;
}

auto pListView::append(sListViewHeader header) -> void {
  lock();
  if(auto self = header->self()) {
    self->_setState();
  }
  unlock();
}

auto pListView::append(sListViewItem item) -> void {
  lock();
  if(auto self = item->self()) {
    self->qtItem = new QTreeWidgetItem(qtListView);
    self->_setState();
  }
  unlock();
}

auto pListView::remove(sListViewHeader header) -> void {
}

auto pListView::remove(sListViewItem item) -> void {
}

auto pListView::resizeColumns() -> void {
  lock();

  if(auto& header = state().header) {
    vector<signed> widths;
    signed minimumWidth = 0;
    signed expandable = 0;
    for(auto column : range(header->columnCount())) {
      signed width = _width(column);
      widths.append(width);
      minimumWidth += width;
      if(header->column(column).expandable()) expandable++;
    }

    signed maximumWidth = self().geometry().width() - 6;
    if(auto scrollBar = qtListView->verticalScrollBar()) {
      if(scrollBar->isVisible()) maximumWidth -= scrollBar->geometry().width();
    }

    signed expandWidth = 0;
    if(expandable && maximumWidth > minimumWidth) {
      expandWidth = (maximumWidth - minimumWidth) / expandable;
    }

    for(auto column : range(header->columnCount())) {
      signed width = widths[column];
      if(header->column(column).expandable()) width += expandWidth;
      qtListView->setColumnWidth(column, width);
    }
  }

  unlock();
}

auto pListView::setAlignment(Alignment alignment) -> void {
}

auto pListView::setBackgroundColor(Color color) -> void {
  if(color) {
    QPalette palette = qtListView->palette();
    palette.setColor(QPalette::Base, QColor(color.red(), color.green(), color.blue()));
    palette.setColor(QPalette::AlternateBase, QColor(max(0, (signed)color.red() - 17), max(0, (signed)color.green() - 17), max(0, (signed)color.blue() - 17)));
    qtListView->setPalette(palette);
    qtListView->setAutoFillBackground(true);
  } else {
    //todo: set default color
  }
}

auto pListView::setBatchable(bool batchable) -> void {
  lock();
  qtListView->setSelectionMode(batchable ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);
  unlock();
}

auto pListView::setBordered(bool bordered) -> void {
  qtListView->repaint();
}

auto pListView::setForegroundColor(Color color) -> void {
  if(color) {
    QPalette palette = qtListView->palette();
    palette.setColor(QPalette::Text, QColor(color.red(), color.green(), color.blue()));
    qtListView->setPalette(palette);
  } else {
    //todo: set default color
  }
}

//called on resize/show events
auto pListView::_onSize() -> void {
  //resize columns only if at least one column is expandable
  if(auto& header = state().header) {
    for(auto& column : header->state.columns) {
      if(column->expandable()) return resizeColumns();
    }
  }
}

auto pListView::_width(unsigned column) -> unsigned {
  if(auto& header = state().header) {
    if(auto width = header->column(column).width()) return width;
    unsigned width = 1;
    if(!header->column(column).visible()) return width;
    if(header->visible()) width = max(width, _widthOfColumn(column));
    for(auto row : range(state().items)) {
      width = max(width, _widthOfCell(row, column));
    }
    return width;
  }
  return 1;
}

auto pListView::_widthOfColumn(unsigned _column) -> unsigned {
  unsigned width = 8;
  if(auto& header = state().header) {
    if(auto column = header->column(_column)) {
      if(auto& icon = column->state.icon) {
        width += icon.width() + 2;
      }
      if(auto& text = column->state.text) {
        width += Font::size(column->font(true), text).width();
      }
    }
  }
  return width;
}

auto pListView::_widthOfCell(unsigned _row, unsigned _column) -> unsigned {
  unsigned width = 8;
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(cell->state.checkable) {
        width += 16 + 2;
      }
      if(auto& icon = cell->state.icon) {
        width += icon.width() + 2;
      }
      if(auto& text = cell->state.text) {
        width += Font::size(cell->font(true), text).width();
      }
    }
  }
  return width;
}

auto QtListView::onActivate() -> void {
  if(!p.locked()) p.self().doActivate();
}

auto QtListView::onChange() -> void {
  for(auto& item : p.state().items) {
    item->state.selected = false;
    if(auto self = item->self()) {
      if(self->qtItem->isSelected()) item->state.selected = true;
    }
  }
  if(!p.locked()) p.self().doChange();
}

auto QtListView::onContext() -> void {
  if(!p.locked()) p.self().doContext();
}

auto QtListView::onSort(int columnNumber) -> void {
  if(auto& header = p.state().header) {
    if(auto column = header->column(columnNumber)) {
      if(!p.locked() && column.sortable()) p.self().doSort(column);
    }
  }
}

auto QtListView::onToggle(QTreeWidgetItem* qtItem, int column) -> void {
  for(auto& item : p.state().items) {
    if(auto self = item->self()) {
      if(qtItem == self->qtItem) {
        if(auto cell = item->cell(column)) {
          cell->state.checked = (qtItem->checkState(column) == Qt::Checked);
          if(!p.locked()) p.self().doToggle(cell);
        }
      }
    }
  }
}

auto QtListView::mousePressEvent(QMouseEvent* event) -> void {
  QTreeWidget::mousePressEvent(event);
  if(event->button() == Qt::RightButton) onContext();
}

auto QtListView::resizeEvent(QResizeEvent* event) -> void {
  QTreeWidget::resizeEvent(event);
  p._onSize();
}

auto QtListView::showEvent(QShowEvent* event) -> void {
  QTreeWidget::showEvent(event);
  p._onSize();
}

QtListViewDelegate::QtListViewDelegate(pListView& p) : QStyledItemDelegate(p.qtListView), p(p) {
}

auto QtListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const -> void {
  QStyledItemDelegate::paint(painter, option, index);
  if(p.state().bordered) {
    QPen pen;
    pen.setColor(QColor(160, 160, 160));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(option.rect);
  }
}

}

#endif
