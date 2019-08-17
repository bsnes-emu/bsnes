#if defined(Hiro_TableView)

auto mTableViewItem::allocate() -> pObject* {
  return new pTableViewItem(*this);
}

auto mTableViewItem::destruct() -> void {
  for(auto& cell : state.cells) cell->destruct();
  mObject::destruct();
}

//

auto mTableViewItem::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableViewItem::append(sTableViewCell cell) -> type& {
  state.cells.append(cell);
  cell->setParent(this, cellCount() - 1);
  signal(append, cell);
  return *this;
}

auto mTableViewItem::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mTableViewItem::cell(unsigned position) const -> TableViewCell {
  if(position < cellCount()) return state.cells[position];
  return {};
}

auto mTableViewItem::cellCount() const -> unsigned {
  return state.cells.size();
}

auto mTableViewItem::cells() const -> vector<TableViewCell> {
  vector<TableViewCell> cells;
  for(auto& cell : state.cells) cells.append(cell);
  return cells;
}

auto mTableViewItem::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mTableViewItem::remove() -> type& {
  if(auto tableView = parentTableView()) tableView->remove(*this);
  return *this;
}

auto mTableViewItem::remove(sTableViewCell cell) -> type& {
  signal(remove, cell);
  state.cells.remove(cell->offset());
  for(auto n : range(cell->offset(), cellCount())) {
    state.cells[n]->adjustOffset(-1);
  }
  cell->setParent();
  return *this;
}

auto mTableViewItem::reset() -> type& {
  while(state.cells) remove(state.cells.right());
  return *this;
}

auto mTableViewItem::selected() const -> bool {
  return state.selected;
}

auto mTableViewItem::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mTableViewItem::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTableViewItem::setFocused() -> type& {
  signal(setFocused);
  return *this;
}

auto mTableViewItem::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTableViewItem::setParent(mObject* parent, signed offset) -> type& {
  for(auto& cell : state.cells) cell->destruct();
  mObject::setParent(parent, offset);
  for(auto& cell : state.cells) cell->setParent(this, cell->offset());
  return *this;
}

auto mTableViewItem::setSelected(bool selected) -> type& {
  //if in single-selection mode, selecting one item must deselect all other items in the TableView
  if(auto parent = parentTableView()) {
    if(!parent->state.batchable && selected) {
      for(auto& item : parent->state.items) item->state.selected = false;
    }
  }
  state.selected = selected;
  signal(setSelected, selected);
  return *this;
}

#endif
