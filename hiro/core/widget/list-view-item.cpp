auto mListViewItem::allocate() -> pObject* {
  return new pListViewItem(*this);
}

//

auto mListViewItem::checked() const -> bool {
  return state.checked;
}

auto mListViewItem::icon(unsigned column) const -> image {
  return state.icon(column, {});
}

auto mListViewItem::remove() -> type& {
  if(auto listView = parentListView()) listView->remove(*this);
  return *this;
}

auto mListViewItem::selected() const -> bool {
  return state.selected;
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

auto mListViewItem::setIcon(unsigned column, const image& icon) -> type& {
  state.icon(column) = icon;
  signal(setIcon, column, icon);
  return *this;
}

auto mListViewItem::setSelected(bool selected) -> type& {
  state.selected = selected;
  signal(setSelected, selected);
  return *this;
}

auto mListViewItem::setText(const lstring& text) -> type& {
  state.text = text;
  if(auto listView = parentListView()) {
    for(auto column : range(listView->columns())) {
      setText(column, text(column, ""));
    }
  }
  return *this;
}

auto mListViewItem::setText(unsigned column, const string& text) -> type& {
  state.text(column) = text;
  signal(setText, column, text);
  return *this;
}

auto mListViewItem::text(unsigned column) const -> string {
  return state.text(column, "");
}
