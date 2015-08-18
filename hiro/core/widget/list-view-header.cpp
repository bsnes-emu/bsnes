#if defined(Hiro_ListView)

auto mListViewHeader::allocate() -> pObject* {
  return new pListViewHeader(*this);
}

//

auto mListViewHeader::append(sListViewColumn column) -> type& {
  state.columns.append(column);
  column->setParent(this, columnCount() - 1);
  signal(append, column);
  return *this;
}

auto mListViewHeader::column(unsigned position) const -> ListViewColumn {
  if(position < columnCount()) return state.columns[position];
  return {};
}

auto mListViewHeader::columnCount() const -> unsigned {
  return state.columns.size();
}

auto mListViewHeader::columns() const -> vector<ListViewColumn> {
  vector<ListViewColumn> columns;
  for(auto& column : state.columns) columns.append(column);
  return columns;
}

auto mListViewHeader::remove() -> type& {
  if(auto listView = parentListView()) listView->remove(*this);
  return *this;
}

auto mListViewHeader::remove(sListViewColumn column) -> type& {
  signal(remove, column);
  state.columns.remove(column->offset());
  for(auto n : range(column->offset(), columnCount())) {
    state.columns[n]->adjustOffset(-1);
  }
  column->setParent();
  return *this;
}

auto mListViewHeader::setParent(mObject* parent, signed offset) -> type& {
  for(auto& column : state.columns) column->destruct();
  mObject::setParent(parent, offset);
  for(auto& column : state.columns) column->setParent(this, column->offset());
  return *this;
}

#endif
