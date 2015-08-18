#if defined(Hiro_ListView)

auto mListView::allocate() -> pObject* {
  return new pListView(*this);
}

auto mListView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  if(auto& header = state.header) header->destruct();
  mWidget::destruct();
}

//

auto mListView::alignment() const -> Alignment {
  return state.alignment;
}

auto mListView::append(sListViewHeader header) -> type& {
  if(auto& header = state.header) remove(header);
  state.header = header;
  header->setParent(this, 0);
  signal(append, header);
  return *this;
}

auto mListView::append(sListViewItem item) -> type& {
  state.items.append(item);
  item->setParent(this, itemCount() - 1);
  signal(append, item);
  return *this;
}

auto mListView::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mListView::batchable() const -> bool {
  return state.batchable;
}

auto mListView::batched() const -> vector<ListViewItem> {
  vector<ListViewItem> items;
  for(auto& item : state.items) {
    if(item->selected()) items.append(item);
  }
  return items;
}

auto mListView::bordered() const -> bool {
  return state.bordered;
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

auto mListView::doToggle(sListViewCell cell) const -> void {
  if(state.onToggle) return state.onToggle(cell);
}

auto mListView::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mListView::header() const -> ListViewHeader {
  return state.header;
}

auto mListView::item(unsigned position) const -> ListViewItem {
  if(position < itemCount()) return state.items[position];
  return {};
}

auto mListView::itemCount() const -> unsigned {
  return state.items.size();
}

auto mListView::items() const -> vector<ListViewItem> {
  vector<ListViewItem> items;
  for(auto& item : state.items) items.append(item);
  return items;
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

auto mListView::onToggle(const function<void (ListViewCell)>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mListView::remove(sListViewHeader header) -> type& {
  signal(remove, header);
  header->setParent();
  state.header.reset();
  return *this;
}

auto mListView::remove(sListViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mListView::reset() -> type& {
  signal(reset);
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  if(auto& header = state.header) header->setParent();
  state.header.reset();
  return *this;
}

auto mListView::resizeColumns() -> type& {
  signal(resizeColumns);
  return *this;
}

auto mListView::selected() const -> ListViewItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {};
}

auto mListView::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
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

auto mListView::setBordered(bool bordered) -> type& {
  state.bordered = bordered;
  signal(setBordered, bordered);
  return *this;
}

auto mListView::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mListView::setParent(mObject* parent, signed offset) -> type& {
  for(auto& item : state.items) item->destruct();
  if(auto& header = state.header) header->destruct();
  mObject::setParent(parent, offset);
  if(auto& header = state.header) header->setParent(this, 0);
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

#endif
