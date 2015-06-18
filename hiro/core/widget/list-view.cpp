#if defined(Hiro_ListView)

auto mListView::allocate() -> pObject* {
  return new pListView(*this);
}

auto mListView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  for(auto& column : state.columns) column->destruct();
  mWidget::destruct();
}

//

auto mListView::append(sListViewColumn column) -> type& {
  state.columns.append(column);
  column->setParent(this, columns() - 1);
  signal(append, column);
  return *this;
}

auto mListView::append(sListViewItem item) -> type& {
  state.items.append(item);
  item->setParent(this, items() - 1);
  signal(append, item);
  return *this;
}

auto mListView::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mListView::batchable() const -> bool {
  return state.batchable;
}

auto mListView::checkable() const -> bool {
  return state.checkable;
}

auto mListView::checkAll() -> type& {
  for(auto& item : state.items) item->state.checked = true;
  signal(checkAll);
  return *this;
}

auto mListView::checked() const -> vector<ListViewItem> {
  vector<ListViewItem> items;
  for(auto& item : state.items) {
    if(item->checked()) items.append(item);
  }
  return items;
}

auto mListView::column(unsigned position) const -> ListViewColumn {
  if(position < columns()) return state.columns[position];
  return {};
}

auto mListView::columns() const -> unsigned {
  return state.columns.size();
}

auto mListView::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mListView::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mListView::doContext() const -> void {
  if(state.onContext) return state.onContext();
}

auto mListView::doEdit(sListViewCell cell) const -> void {
  if(state.onEdit) return state.onEdit(cell);
}

auto mListView::doSort(sListViewColumn column) const -> void {
  if(state.onSort) return state.onSort(column);
}

auto mListView::doToggle(sListViewItem item) const -> void {
  if(state.onToggle) return state.onToggle(item);
}

auto mListView::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mListView::gridVisible() const -> bool {
  return state.gridVisible;
}

auto mListView::headerVisible() const -> bool {
  return state.headerVisible;
}

auto mListView::item(unsigned position) const -> ListViewItem {
  if(position < items()) return state.items[position];
  return {};
}

auto mListView::items() const -> unsigned {
  return state.items.size();
}

auto mListView::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mListView::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mListView::onContext(const function<void ()>& callback) -> type& {
  state.onContext = callback;
  return *this;
}

auto mListView::onEdit(const function<void (ListViewCell)>& callback) -> type& {
  state.onEdit = callback;
  return *this;
}

auto mListView::onSort(const function<void (ListViewColumn)>& callback) -> type& {
  state.onSort = callback;
  return *this;
}

auto mListView::onToggle(const function<void (ListViewItem)>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mListView::remove(sListViewColumn column) -> type& {
  signal(remove, column);
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  state.columns.remove(column->offset());
  for(auto n : range(column->offset(), columns())) {
    state.columns[n]->adjustOffset(-1);
  }
  column->setParent();
  return *this;
}

auto mListView::remove(sListViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), items())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mListView::reset() -> type& {
  signal(reset);
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  for(auto& column : state.columns) column->setParent();
  state.columns.reset();
  return *this;
}

auto mListView::resizeColumns() -> type& {
  signal(resizeColumns);
  return *this;
}

auto mListView::selectAll() -> type& {
  for(auto& item : state.items) item->state.selected = true;
  signal(selectAll);
  return *this;
}

auto mListView::selected() const -> ListViewItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {nullptr};
}

auto mListView::selectedItems() const -> vector<ListViewItem> {
  vector<ListViewItem> items;
  for(auto& item : state.items) {
    if(item->selected()) items.append(item);
  }
  return items;
}

auto mListView::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mListView::setBatchable(bool batchable) -> type& {
  state.batchable = batchable;
  signal(setBatchable, batchable);
  return *this;
}

auto mListView::setCheckable(bool checkable) -> type& {
  state.checkable = checkable;
  signal(setCheckable, checkable);
  return *this;
}

auto mListView::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mListView::setGridVisible(bool visible) -> type& {
  state.gridVisible = visible;
  signal(setGridVisible, visible);
  return *this;
}

auto mListView::setHeaderVisible(bool visible) -> type& {
  state.headerVisible = visible;
  signal(setHeaderVisible, visible);
  return *this;
}

auto mListView::setParent(mObject* parent, signed offset) -> type& {
  for(auto& item : state.items) item->destruct();
  for(auto& column : state.columns) column->destruct();
  mObject::setParent(parent, offset);
  for(auto& column : state.columns) column->setParent(this, column->offset());
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

auto mListView::setSortable(bool sortable) -> type& {
  state.sortable = sortable;
  signal(setSortable, sortable);
  return *this;
}

auto mListView::sortable() const -> bool {
  return state.sortable;
}

auto mListView::uncheckAll() -> type& {
  for(auto& item : state.items) item->state.checked = false;
  signal(uncheckAll);
  return *this;
}

auto mListView::unselectAll() -> type& {
  for(auto& item : state.items) item->state.selected = false;
  signal(unselectAll);
  return *this;
}

#endif
