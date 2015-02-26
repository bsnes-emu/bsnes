auto mTreeViewItem::allocate() -> pObject* {
  return new pTreeViewItem(*this);
}

auto mTreeViewItem::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mObject::destruct();
}

//

auto mTreeViewItem::append(sTreeViewItem item) -> type& {
  state.items.append(item);
  item->setParent(this, items() - 1);
  signal(append, item);
  return *this;
}

auto mTreeViewItem::checked() const -> bool {
  return state.checked;
}

auto mTreeViewItem::icon() const -> image {
  return state.icon;
}

auto mTreeViewItem::item(const string& path) const -> sTreeViewItem {
  if(path.empty()) return {};
  auto paths = path.split("/");
  unsigned position = decimal(paths.takeFirst());
  if(position >= items()) return {};
  if(paths.empty()) return state.items[position];
  return state.items[position]->item(paths.merge("/"));
}

auto mTreeViewItem::items() const -> unsigned {
  return state.items.size();
}

auto mTreeViewItem::path() const -> string {
  if(auto treeViewItem = parentTreeViewItem()) return {treeViewItem->path(), "/", offset()};
  return {offset()};
}

auto mTreeViewItem::remove() -> type& {
  if(auto treeView = parentTreeView()) treeView->remove(*this);
  if(auto treeViewItem = parentTreeViewItem()) treeViewItem->remove(*this);
  return *this;
}

auto mTreeViewItem::remove(sTreeViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), items())) {
    state.items[n]->offset(-1);
  }
  item->setParent();
  return *this;
}

auto mTreeViewItem::selected() const -> bool {
  if(auto treeView = parentTreeView(true)) {
    return path() == treeView->state.selectedPath;
  }
  return false;
}

auto mTreeViewItem::setChecked(bool checked) -> type& {
  state.checked = checked;
  signal(setChecked, checked);
  return *this;
}

auto mTreeViewItem::setFocused() -> type& {
  signal(setFocused);
  return *this;
}

auto mTreeViewItem::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mTreeViewItem::setSelected() -> type& {
  signal(setSelected);
  return *this;
}

auto mTreeViewItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mTreeViewItem::text() const -> string {
  return state.text;
}
