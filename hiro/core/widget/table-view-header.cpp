#if defined(Hiro_TableView)

auto mTableViewHeader::allocate() -> pObject* {
  return new pTableViewHeader(*this);
}

//

auto mTableViewHeader::append(sTableViewColumn column) -> type& {
  state.columns.append(column);
  column->setParent(this, columnCount() - 1);
  signal(append, column);
  return *this;
}

auto mTableViewHeader::column(unsigned position) const -> TableViewColumn {
  if(position < columnCount()) return state.columns[position];
  return {};
}

auto mTableViewHeader::columnCount() const -> unsigned {
  return state.columns.size();
}

auto mTableViewHeader::columns() const -> vector<TableViewColumn> {
  vector<TableViewColumn> columns;
  for(auto& column : state.columns) columns.append(column);
  return columns;
}

auto mTableViewHeader::remove() -> type& {
  if(auto tableView = parentTableView()) tableView->remove(*this);
  return *this;
}

auto mTableViewHeader::remove(sTableViewColumn column) -> type& {
  signal(remove, column);
  state.columns.remove(column->offset());
  for(auto n : range(column->offset(), columnCount())) {
    state.columns[n]->adjustOffset(-1);
  }
  column->setParent();
  return *this;
}

auto mTableViewHeader::reset() -> type& {
  for(auto n : rrange(state.columns)) remove(state.columns[n]);
  return *this;
}

auto mTableViewHeader::setParent(mObject* parent, signed offset) -> type& {
  for(auto& column : state.columns) column->destruct();
  mObject::setParent(parent, offset);
  for(auto& column : state.columns) column->setParent(this, column->offset());
  return *this;
}

#endif
