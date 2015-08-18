#if defined(Hiro_ListView)

auto mListViewItem::allocate() -> pObject* {
  return new pListViewItem(*this);
}

//

auto mListViewItem::alignment() const -> Alignment {
  return state.alignment;
}

auto mListViewItem::append(sListViewCell cell) -> type& {
  state.cells.append(cell);
  cell->setParent(this, cellCount() - 1);
  signal(append, cell);
  return *this;
}

auto mListViewItem::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mListViewItem::cell(unsigned position) const -> ListViewCell {
  if(position < cellCount()) return state.cells[position];
  return {};
}

auto mListViewItem::cellCount() const -> unsigned {
  return state.cells.size();
}

auto mListViewItem::cells() const -> vector<ListViewCell> {
  vector<ListViewCell> cells;
  for(auto& cell : state.cells) cells.append(cell);
  return cells;
}

auto mListViewItem::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mListViewItem::remove() -> type& {
  if(auto listView = parentListView()) listView->remove(*this);
  return *this;
}

auto mListViewItem::remove(sListViewCell cell) -> type& {
  signal(remove, cell);
  state.cells.remove(cell->offset());
  for(auto n : range(cell->offset(), cellCount())) {
    state.cells[n]->adjustOffset(-1);
  }
  cell->setParent();
  return *this;
}

auto mListViewItem::selected() const -> bool {
  return state.selected;
}

auto mListViewItem::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mListViewItem::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mListViewItem::setFocused() -> type& {
  signal(setFocused);
  return *this;
}

auto mListViewItem::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mListViewItem::setParent(mObject* parent, signed offset) -> type& {
  for(auto& cell : state.cells) cell->destruct();
  mObject::setParent(parent, offset);
  for(auto& cell : state.cells) cell->setParent(this, cell->offset());
  return *this;
}

auto mListViewItem::setSelected(bool selected) -> type& {
  state.selected = selected;
  signal(setSelected, selected);
  return *this;
}

#endif
