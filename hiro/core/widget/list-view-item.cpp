#if defined(Hiro_ListView)

auto mListViewItem::allocate() -> pObject* {
  return new pListViewItem(*this);
}

//

auto mListViewItem::append(sListViewCell cell) -> type& {
  state.cells.append(cell);
  cell->setParent(this, cells() - 1);
  signal(append, cell);
  return *this;
}

auto mListViewItem::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mListViewItem::cell(unsigned position) const -> sListViewCell {
  if(position < cells()) return state.cells[position];
  return {};
}

auto mListViewItem::cells() const -> unsigned {
  return state.cells.size();
}

auto mListViewItem::checked() const -> bool {
  return state.checked;
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
  for(auto n : range(cell->offset(), cells())) {
    state.cells[n]->adjustOffset(-1);
  }
  cell->setParent();
  return *this;
}

auto mListViewItem::selected() const -> bool {
  return state.selected;
}

auto mListViewItem::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mListViewItem::setChecked(bool checked) -> type& {
  state.checked = checked;
  signal(setChecked, checked);
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
