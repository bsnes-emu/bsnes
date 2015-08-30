#if defined(Hiro_TreeView)

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
  item->setParent(this, itemCount() - 1);
  signal(append, item);
  return *this;
}

auto mTreeViewItem::backgroundColor(bool recursive) const -> Color {
  if(auto color = state.backgroundColor) return color;
  if(recursive) {
    if(auto parent = parentTreeViewItem()) {
      if(auto color = parent->backgroundColor(true)) return color;
    }
    if(auto parent = parentTreeView()) {
      if(auto color = parent->backgroundColor()) return color;
    }
  }
  return {};
}

auto mTreeViewItem::checkable() const -> bool {
  return state.checkable;
}

auto mTreeViewItem::checked() const -> bool {
  return state.checked;
}

auto mTreeViewItem::foregroundColor(bool recursive) const -> Color {
  if(auto color = state.foregroundColor) return color;
  if(recursive) {
    if(auto parent = parentTreeViewItem()) {
      if(auto color = parent->foregroundColor(true)) return color;
    }
    if(auto parent = parentTreeView()) {
      if(auto color = parent->foregroundColor()) return color;
    }
  }
  return {};
}

auto mTreeViewItem::image() const -> Image {
  return state.image;
}

auto mTreeViewItem::item(const string& path) const -> TreeViewItem {
  if(path.empty()) return {};
  auto paths = path.split("/");
  unsigned position = decimal(paths.takeFirst());
  if(position >= itemCount()) return {};
  if(paths.empty()) return state.items[position];
  return state.items[position]->item(paths.merge("/"));
}

auto mTreeViewItem::itemCount() const -> unsigned {
  return state.items.size();
}

auto mTreeViewItem::items() const -> vector<TreeViewItem> {
  vector<TreeViewItem> items;
  for(auto& item : state.items) items.append(item);
  return items;
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
  for(auto n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
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

auto mTreeViewItem::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTreeViewItem::setCheckable(bool checkable) -> type& {
  state.checkable = checkable;
  signal(setCheckable, checkable);
  return *this;
}

auto mTreeViewItem::setChecked(bool checked) -> type& {
  state.checked = checked;
  signal(setChecked, checked);
  return *this;
}

auto mTreeViewItem::setExpanded(bool expanded) -> type& {
  signal(setExpanded, expanded);
  return *this;
}

auto mTreeViewItem::setFocused() -> type& {
  signal(setFocused);
  return *this;
}

auto mTreeViewItem::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTreeViewItem::setImage(const Image& image) -> type& {
  state.image = image;
  signal(setImage, image);
  return *this;
}

auto mTreeViewItem::setParent(mObject* parent, signed offset) -> type& {
  for(auto n : rrange(state.items)) state.items[n]->destruct();
  mObject::setParent(parent, offset);
  for(auto& item : state.items) item->setParent(this, item->offset());
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

#endif
