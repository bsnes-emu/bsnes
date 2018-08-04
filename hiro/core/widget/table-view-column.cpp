#if defined(Hiro_TableView)

auto mTableViewColumn::allocate() -> pObject* {
  return new pTableViewColumn(*this);
}

//

//TODO: active(), setActive() ... is this the best way to do this?

auto mTableViewColumn::active() const -> bool {
  if(auto tableView = parentTableView(true)) return tableView->state.activeColumn == offset();
  return false;
}

auto mTableViewColumn::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableViewColumn::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mTableViewColumn::editable() const -> bool {
  return state.editable;
}

auto mTableViewColumn::expandable() const -> bool {
  return state.expandable;
}

auto mTableViewColumn::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mTableViewColumn::horizontalAlignment() const -> float {
  return state.horizontalAlignment;
}

auto mTableViewColumn::icon() const -> image {
  return state.icon;
}

auto mTableViewColumn::remove() -> type& {
  if(auto tableView = parentTableView()) tableView->remove(*this);
  return *this;
}

auto mTableViewColumn::resizable() const -> bool {
  return state.resizable;
}

auto mTableViewColumn::setActive() -> type& {
  if(auto tableView = parentTableView(true)) tableView->state.activeColumn = offset();
  signal(setActive);
  return *this;
}

auto mTableViewColumn::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mTableViewColumn::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTableViewColumn::setEditable(bool editable) -> type& {
  state.editable = editable;
  signal(setEditable, editable);
  return *this;
}

auto mTableViewColumn::setExpandable(bool expandable) -> type& {
  state.expandable = expandable;
  signal(setExpandable, expandable);
  return *this;
}

auto mTableViewColumn::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTableViewColumn::setHorizontalAlignment(float alignment) -> type& {
  alignment = max(0.0, min(1.0, alignment));
  state.horizontalAlignment = alignment;
  signal(setHorizontalAlignment, alignment);
  return *this;
}

auto mTableViewColumn::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mTableViewColumn::setResizable(bool resizable) -> type& {
  state.resizable = resizable;
  signal(setResizable, resizable);
  return *this;
}

auto mTableViewColumn::setSorting(Sort sorting) -> type& {
  if(auto tableView = parentTableView()) {
    for(auto& column : tableView->state.columns) {
      column->state.sorting = Sort::None;
      signalex(column, setSorting, Sort::None);
    }
  }
  state.sorting = sorting;
  signal(setSorting, sorting);
  return *this;
}

auto mTableViewColumn::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mTableViewColumn::setVerticalAlignment(float alignment) -> type& {
  alignment = max(0.0, min(1.0, alignment));
  state.verticalAlignment = alignment;
  signal(setVerticalAlignment, alignment);
  return *this;
}

auto mTableViewColumn::setVisible(bool visible) -> type& {
  state.visible = visible;
  signal(setVisible, visible);
  return *this;
}

auto mTableViewColumn::setWidth(float width) -> type& {
  state.width = max(0, width);
  signal(setWidth, width);
  return *this;
}

auto mTableViewColumn::sorting() const -> Sort {
  return state.sorting;
}

auto mTableViewColumn::text() const -> string {
  return state.text;
}

auto mTableViewColumn::verticalAlignment() const -> float {
  return state.verticalAlignment;
}

auto mTableViewColumn::width() const -> float {
  return state.width;
}

#endif
