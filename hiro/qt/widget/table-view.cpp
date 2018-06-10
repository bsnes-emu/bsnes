#if defined(Hiro_TableView)

namespace hiro {

auto pTableView::construct() -> void {
  qtWidget = qtTableView = new QtTableView(*this);
  qtTableView->setAllColumnsShowFocus(true);
  qtTableView->setContextMenuPolicy(Qt::CustomContextMenu);
  qtTableView->setRootIsDecorated(false);
  qtTableView->setHeaderHidden(true);
  #if HIRO_QT==4
  qtTableView->header()->setMovable(false);
  #elif HIRO_QT==5
  qtTableView->header()->setSectionsMovable(false);
  #endif

  qtTableViewDelegate = new QtTableViewDelegate(*this);
  qtTableView->setItemDelegate(qtTableViewDelegate);

  qtTableView->connect(qtTableView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate()));
  qtTableView->connect(qtTableView, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  qtTableView->connect(qtTableView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onContext()));
  qtTableView->connect(qtTableView->header(), SIGNAL(sectionClicked(int)), SLOT(onSort(int)));
  qtTableView->connect(qtTableView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onToggle(QTreeWidgetItem*, int)));

  setBackgroundColor(state().backgroundColor);
  setBatchable(state().batchable);
  setBordered(state().bordered);
  setForegroundColor(state().foregroundColor);

  pWidget::construct();
}

auto pTableView::destruct() -> void {
  delete qtTableViewDelegate;
  delete qtTableView;
  qtWidget = qtTableView = nullptr;
  qtTableViewDelegate = nullptr;
}

auto pTableView::append(sTableViewHeader header) -> void {
  lock();
  if(auto self = header->self()) {
    self->_setState();
  }
  unlock();
}

auto pTableView::append(sTableViewItem item) -> void {
  lock();
  if(auto self = item->self()) {
    self->qtItem = new QTreeWidgetItem(qtTableView);
    self->_setState();
  }
  unlock();
}

auto pTableView::remove(sTableViewHeader header) -> void {
}

auto pTableView::remove(sTableViewItem item) -> void {
}

auto pTableView::resizeColumns() -> void {
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
    if(auto scrollBar = qtTableView->verticalScrollBar()) {
      if(scrollBar->isVisible()) maximumWidth -= scrollBar->geometry().width();
    }

    signed expandWidth = 0;
    if(expandable && maximumWidth > minimumWidth) {
      expandWidth = (maximumWidth - minimumWidth) / expandable;
    }

    for(auto column : range(header->columnCount())) {
      signed width = widths[column];
      if(header->column(column).expandable()) width += expandWidth;
      qtTableView->setColumnWidth(column, width);
    }
  }

  unlock();
}

auto pTableView::setAlignment(Alignment alignment) -> void {
}

auto pTableView::setBackgroundColor(Color color) -> void {
  if(color) {
    QPalette palette = qtTableView->palette();
    palette.setColor(QPalette::Base, QColor(color.red(), color.green(), color.blue()));
    palette.setColor(QPalette::AlternateBase, QColor(max(0, (signed)color.red() - 17), max(0, (signed)color.green() - 17), max(0, (signed)color.blue() - 17)));
    qtTableView->setPalette(palette);
    qtTableView->setAutoFillBackground(true);
  } else {
    //todo: set default color
  }
}

auto pTableView::setBatchable(bool batchable) -> void {
  lock();
  qtTableView->setSelectionMode(batchable ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);
  unlock();
}

auto pTableView::setBordered(bool bordered) -> void {
  qtTableView->repaint();
}

auto pTableView::setForegroundColor(Color color) -> void {
  if(color) {
    QPalette palette = qtTableView->palette();
    palette.setColor(QPalette::Text, QColor(color.red(), color.green(), color.blue()));
    qtTableView->setPalette(palette);
  } else {
    //todo: set default color
  }
}

//called on resize/show events
auto pTableView::_onSize() -> void {
  //resize columns only if at least one column is expandable
  if(auto& header = state().header) {
    for(auto& column : header->state.columns) {
      if(column->expandable()) return resizeColumns();
    }
  }
}

auto pTableView::_width(unsigned column) -> unsigned {
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

auto pTableView::_widthOfColumn(unsigned _column) -> unsigned {
  unsigned width = 8;
  if(auto& header = state().header) {
    if(auto column = header->column(_column)) {
      if(auto& icon = column->state.icon) {
        width += icon.width() + 4;
      }
      if(auto& text = column->state.text) {
        width += pFont::size(column->font(true), text).width();
      }
    }
  }
  return width;
}

auto pTableView::_widthOfCell(unsigned _row, unsigned _column) -> unsigned {
  unsigned width = 8;
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(cell->state.checkable) {
        width += 16 + 4;
      }
      if(auto& icon = cell->state.icon) {
        width += icon.width() + 4;
      }
      if(auto& text = cell->state.text) {
        width += pFont::size(cell->font(true), text).width();
      }
    }
  }
  return width;
}

auto QtTableView::onActivate() -> void {
  if(!p.locked()) p.self().doActivate();
}

auto QtTableView::onChange() -> void {
  for(auto& item : p.state().items) {
    item->state.selected = false;
    if(auto self = item->self()) {
      if(self->qtItem->isSelected()) item->state.selected = true;
    }
  }
  if(!p.locked()) p.self().doChange();
}

auto QtTableView::onContext() -> void {
  if(!p.locked()) p.self().doContext();
}

auto QtTableView::onSort(int columnNumber) -> void {
  if(auto& header = p.state().header) {
    if(auto column = header->column(columnNumber)) {
      if(!p.locked() && column.sortable()) p.self().doSort(column);
    }
  }
}

auto QtTableView::onToggle(QTreeWidgetItem* qtItem, int column) -> void {
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

auto QtTableView::mousePressEvent(QMouseEvent* event) -> void {
  QTreeWidget::mousePressEvent(event);
  if(event->button() == Qt::RightButton) onContext();
}

auto QtTableView::resizeEvent(QResizeEvent* event) -> void {
  QTreeWidget::resizeEvent(event);
  p._onSize();
}

auto QtTableView::showEvent(QShowEvent* event) -> void {
  QTreeWidget::showEvent(event);
  p._onSize();
}

QtTableViewDelegate::QtTableViewDelegate(pTableView& p) : QStyledItemDelegate(p.qtTableView), p(p) {
}

auto QtTableViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const -> void {
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
