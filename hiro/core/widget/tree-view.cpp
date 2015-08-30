#if defined(Hiro_TreeView)

auto mTreeView::allocate() -> pObject* {
  return new pTreeView(*this);
}

auto mTreeView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mWidget::destruct();
}

//

auto mTreeView::append(sTreeViewItem item) -> type& {
  state.items.append(item);
  item->setParent(this, itemCount() - 1);
  signal(append, item);
  return *this;
}

auto mTreeView::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mTreeView::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mTreeView::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mTreeView::doContext() const -> void {
  if(state.onContext) return state.onContext();
}

auto mTreeView::doToggle(sTreeViewItem item) const -> void {
  if(state.onToggle) return state.onToggle(item);
}

auto mTreeView::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mTreeView::item(const string& path) const -> TreeViewItem {
  if(path.empty()) return {};
  auto paths = path.split("/");
  unsigned position = decimal(paths.takeFirst());
  if(position >= itemCount()) return {};
  if(paths.empty()) return state.items[position];
  return state.items[position]->item(paths.merge("/"));
}

auto mTreeView::itemCount() const -> unsigned {
  return state.items.size();
}

auto mTreeView::items() const -> vector<TreeViewItem> {
  vector<TreeViewItem> items;
  for(auto& item : state.items) items.append(item);
  return items;
}

auto mTreeView::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mTreeView::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mTreeView::onContext(const function<void ()>& callback) -> type& {
  state.onContext = callback;
  return *this;
}

auto mTreeView::onToggle(const function<void (sTreeViewItem)>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mTreeView::remove(sTreeViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mTreeView::reset() -> type& {
  state.selectedPath.reset();
  for(auto n : rrange(state.items)) remove(state.items[n]);
  return *this;
}

auto mTreeView::selected() const -> TreeViewItem {
  return item(state.selectedPath);
}

auto mTreeView::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTreeView::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTreeView::setParent(mObject* object, signed offset) -> type& {
  for(auto n : rrange(state.items)) state.items[n]->destruct();
  mObject::setParent(object, offset);
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

#endif
