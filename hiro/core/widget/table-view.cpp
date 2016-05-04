#if defined(Hiro_TableView)

auto mTableView::allocate() -> pObject* {
  return new pTableView(*this);
}

auto mTableView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  if(auto& header = state.header) header->destruct();
  mWidget::destruct();
}

//

auto mTableView::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableView::append(sTableViewHeader header) -> type& {
  if(auto& header = state.header) remove(header);
  state.header = header;
  header->setParent(this, 0);
  signal(append, header);
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

auto mTableView::header() const -> TableViewHeader {
  return state.header;
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

auto mTableView::remove(sTableViewHeader header) -> type& {
  signal(remove, header);
  header->setParent();
  state.header.reset();
  return *this;
}

auto mTableView::remove(sTableViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mTableView::reset() -> type& {
  for(auto n : rrange(state.items)) remove(state.items[n]);
  if(auto& header = state.header) remove(header);
  return *this;
}

auto mTableView::resizeColumns() -> type& {
  signal(resizeColumns);
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

auto mTableView::setParent(mObject* parent, signed offset) -> type& {
  for(auto n : rrange(state.items)) state.items[n]->destruct();
  if(auto& header = state.header) header->destruct();
  mObject::setParent(parent, offset);
  if(auto& header = state.header) header->setParent(this, 0);
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

#endif
