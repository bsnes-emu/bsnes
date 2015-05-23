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

auto mListView::checkable() const -> bool {
  return state.checkable;
}

auto mListView::checked() const -> vector<sListViewItem> {
  vector<sListViewItem> items;
  for(auto& item : state.items) {
    if(item->checked()) items.append(item);
  }
  return items;
}

auto mListView::column(unsigned position) const -> sListViewColumn {
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

auto mListView::doEdit(sListViewItem item, sListViewColumn column) const -> void {
  if(state.onEdit) return state.onEdit(item, column);
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

auto mListView::item(unsigned position) const -> sListViewItem {
  if(position < items()) return state.items[position];
  return {};
}

auto mListView::items() const -> unsigned {
  return state.items.size();
}

auto mListView::multiSelect() const -> bool {
  return state.multiSelect;
}

auto mListView::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mListView::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mListView::onContext(const function<void ()>& function) -> type& {
  state.onContext = function;
  return *this;
}

auto mListView::onEdit(const function<void (sListViewItem, sListViewColumn)>& function) -> type& {
  state.onEdit = function;
  return *this;
}

auto mListView::onSort(const function<void (sListViewColumn)>& function) -> type& {
  state.onSort = function;
  return *this;
}

auto mListView::onToggle(const function<void (sListViewItem)>& function) -> type& {
  state.onToggle = function;
  return *this;
}

auto mListView::remove(sListViewColumn column) -> type& {
  signal(remove, column);
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  state.columns.remove(column->offset());
  for(auto n : range(column->offset(), columns())) {
    state.columns[n]->offset(-1);
  }
  column->setParent();
  return *this;
}

auto mListView::remove(sListViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), items())) {
    state.items[n]->offset(-1);
  }
  item->setParent();
  return *this;
}

auto mListView::reset() -> type& {
  signal(reset);
  for(auto& column : state.columns) column->setParent();
  state.columns.reset();
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  return *this;
}

auto mListView::resizeColumns() -> type& {
  signal(resizeColumns);
  return *this;
}

auto mListView::selected() const -> sListViewItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {};
}

auto mListView::selectedItems() const -> vector<sListViewItem> {
  vector<sListViewItem> items;
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

auto mListView::setCheckable(bool checkable) -> type& {
  state.checkable = checkable;
  signal(setCheckable, checkable);
  return *this;
}

auto mListView::setChecked(bool checked) -> type& {
  for(auto& item : state.items) item->state.checked = checked;
  signal(setChecked, checked);
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

auto mListView::setMultiSelect(bool multiSelect) -> type& {
  state.multiSelect = multiSelect;
  signal(setMultiSelect, multiSelect);
  return *this;
}

auto mListView::setSelected(bool selected) -> type& {
  for(auto& item : state.items) item->state.selected = selected;
  signal(setSelected, selected);
  return *this;
}

#endif
