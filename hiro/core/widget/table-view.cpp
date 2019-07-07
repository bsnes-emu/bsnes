#if defined(Hiro_TableView)

auto mTableView::allocate() -> pObject* {
  return new pTableView(*this);
}

auto mTableView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  for(auto& column : state.columns) column->destruct();
  mWidget::destruct();
}

//

auto mTableView::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableView::append(sTableViewColumn column) -> type& {
  state.columns.append(column);
  column->setParent(this, columnCount() - 1);
  signal(append, column);
  return *this;
}

auto mTableView::append(sTableViewItem item) -> type& {
  state.items.append(item);
  item->setParent(this, itemCount() - 1);
  signal(append, item);
  return *this;
}

auto mTableView::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mTableView::batchable() const -> bool {
  return state.batchable;
}

auto mTableView::batched() const -> vector<TableViewItem> {
  vector<TableViewItem> items;
  for(auto& item : state.items) {
    if(item->selected()) items.append(item);
  }
  return items;
}

auto mTableView::bordered() const -> bool {
  return state.bordered;
}

auto mTableView::column(uint position) const -> TableViewColumn {
  if(position < columnCount()) return state.columns[position];
  return {};
}

auto mTableView::columnCount() const -> uint {
  return state.columns.size();
}

auto mTableView::columns() const -> vector<TableViewColumn> {
  vector<TableViewColumn> columns;
  for(auto& column : columns) columns.append(column);
  return columns;
}

auto mTableView::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mTableView::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mTableView::doContext() const -> void {
  if(state.onContext) return state.onContext();
}

auto mTableView::doEdit(sTableViewCell cell) const -> void {
  if(state.onEdit) return state.onEdit(cell);
}

auto mTableView::doSort(sTableViewColumn column) const -> void {
  if(state.onSort) return state.onSort(column);
}

auto mTableView::doToggle(sTableViewCell cell) const -> void {
  if(state.onToggle) return state.onToggle(cell);
}

auto mTableView::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mTableView::headered() const -> bool {
  return state.headered;
}

auto mTableView::item(unsigned position) const -> TableViewItem {
  if(position < itemCount()) return state.items[position];
  return {};
}

auto mTableView::itemCount() const -> unsigned {
  return state.items.size();
}

auto mTableView::items() const -> vector<TableViewItem> {
  vector<TableViewItem> items;
  for(auto& item : state.items) items.append(item);
  return items;
}

auto mTableView::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mTableView::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mTableView::onContext(const function<void ()>& callback) -> type& {
  state.onContext = callback;
  return *this;
}

auto mTableView::onEdit(const function<void (TableViewCell)>& callback) -> type& {
  state.onEdit = callback;
  return *this;
}

auto mTableView::onSort(const function<void (TableViewColumn)>& callback) -> type& {
  state.onSort = callback;
  return *this;
}

auto mTableView::onToggle(const function<void (TableViewCell)>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mTableView::remove(sTableViewColumn column) -> type& {
  signal(remove, column);
  state.columns.remove(column->offset());
  for(uint n : range(column->offset(), columnCount())) {
    state.columns[n]->adjustOffset(-1);
  }
  column->setParent();
  return *this;
}

auto mTableView::remove(sTableViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(uint n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mTableView::reset() -> type& {
  while(state.items) remove(state.items.last());
  while(state.columns) remove(state.columns.last());
  return *this;
}

auto mTableView::resizeColumns() -> type& {
  signal(resizeColumns);
  return *this;
}

auto mTableView::selectAll() -> type& {
  if(!state.batchable) return *this;
  for(auto& item : state.items) {
    item->setSelected(true);
  }
  return *this;
}

auto mTableView::selectNone() -> type& {
  for(auto& item : state.items) {
    item->setSelected(false);
  }
  return *this;
}

auto mTableView::selected() const -> TableViewItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {};
}

auto mTableView::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mTableView::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTableView::setBatchable(bool batchable) -> type& {
  state.batchable = batchable;
  signal(setBatchable, batchable);
  return *this;
}

auto mTableView::setBordered(bool bordered) -> type& {
  state.bordered = bordered;
  signal(setBordered, bordered);
  return *this;
}

auto mTableView::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTableView::setHeadered(bool headered) -> type& {
  state.headered = headered;
  signal(setHeadered, headered);
  return *this;
}

auto mTableView::setParent(mObject* parent, signed offset) -> type& {
  for(auto& item : reverse(state.items)) item->destruct();
  for(auto& column : reverse(state.columns)) column->destruct();
  mObject::setParent(parent, offset);
  for(auto& column : state.columns) column->setParent(this, column->offset());
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

auto mTableView::setSortable(bool sortable) -> type& {
  state.sortable = sortable;
  signal(setSortable, sortable);
  return *this;
}

auto mTableView::sort() -> type& {
  Sort sorting = Sort::None;
  uint offset = 0;
  for(auto& column : state.columns) {
    if(column->sorting() == Sort::None) continue;
    sorting = column->sorting();
    offset = column->offset();
    break;
  }
  auto sorted = state.items;
  sorted.sort([&](auto& lhs, auto& rhs) {
    string x = offset < lhs->cellCount() ? lhs->state.cells[offset]->state.text : "";
    string y = offset < rhs->cellCount() ? rhs->state.cells[offset]->state.text : "";
    if(sorting == Sort::Ascending ) return string::icompare(x, y) < 0;
    if(sorting == Sort::Descending) return string::icompare(y, x) < 0;
    return false;  //unreachable
  });
  while(state.items) remove(state.items.last());
  for(auto& item : sorted) append(item);
  return *this;
}

auto mTableView::sortable() const -> bool {
  return state.sortable;
}

#endif
